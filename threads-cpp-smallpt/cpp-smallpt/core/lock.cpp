//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "stdafx.h"
#include "lock.hpp"

#include <limits.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	//-------------------------------------------------------------------------
	// Mutex
	//-------------------------------------------------------------------------
	Mutex::Mutex() {
		// Initialize a critical section object.
		InitializeCriticalSection(&m_critical_section);
	}

	Mutex::~Mutex() {
		// Release all resources used by an unowned critical section object. 
		DeleteCriticalSection(&m_critical_section);
	}

	//-------------------------------------------------------------------------
	// MutexLock
	//-------------------------------------------------------------------------
	MutexLock::MutexLock(Mutex &mutex)
		: m_mutex(mutex) {
		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_mutex.m_critical_section);
	}

	MutexLock::~MutexLock() {
		// Release ownership of the specified critical section object.
		LeaveCriticalSection(&m_mutex.m_critical_section);
	}

	//-------------------------------------------------------------------------
	// Semaphore
	//-------------------------------------------------------------------------
	Semaphore::Semaphore() 
		: m_handle(nullptr) {
		// Creates or opens an unnamed semaphore object.
		// 1. The returned handle cannot be inherited by child processes.
		// 2. The initial count for the semaphore object. 
		// 3. The maximum count for the semaphore object.
		// 4. The semaphore object is created without a name.
		m_handle = CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);
	}

	Semaphore::~Semaphore() {
		// Closes an open object handle.
		CloseHandle(m_handle);
	}

	void Semaphore::Signal(uint32_t count) noexcept {
		// Increases the count of the specified semaphore object.
		// 1. A handle to the semaphore object.
		// 2. The amount by which the semaphore object's current count is to be increased.
		// 3. A pointer to a variable to receive the previous count for the semaphore.
		ReleaseSemaphore(m_handle, count, nullptr);
	}

	void Semaphore::Wait() noexcept {
		// Waits until the specified object is in the signaled state or the time-out interval elapses.
		// 1. The object handle.
		// 2. The function will return only when the specified object is signaled.
		WaitForSingleObject(m_handle, INFINITE);
	}

	bool Semaphore::TryWait() noexcept {
		// Waits until the specified object is in the signaled state or the time-out interval elapses.
		// 1. The object handle.
		// 2. The function does not enter a wait state if the object is not signaled.
		// it always returns immediately.
		return (WaitForSingleObject(m_handle, 0L) == WAIT_OBJECT_0);
	}

	//-------------------------------------------------------------------------
	// ConditionVariable
	//-------------------------------------------------------------------------
	ConditionVariable::ConditionVariable()
		: m_nb_waiters(0) {

		// Initialize the critical section objects
		// for the number of waiters and condition.
		InitializeCriticalSection(&m_nb_waiters_mutex);
		InitializeCriticalSection(&m_condition_mutex);

		// Creates or opens a named or unnamed event object.
		// On success, a handle to the event object is returned.
		m_events[SIGNAL] = CreateEvent(
			nullptr,	// no security
			FALSE,		// auto-reset event object
			FALSE,		// non-signaled initial state
			nullptr);	// unnamed event object
		m_events[BROADCAST] = CreateEvent(
			nullptr,	// no security
			TRUE,		// manual-reset event object
			FALSE,		// non-signaled initial state
			nullptr);	// unnamed event object
	}
	
	ConditionVariable::~ConditionVariable() {
		// Release all resources used by an unowned critical section object. 
		DeleteCriticalSection(&m_nb_waiters_mutex);
		DeleteCriticalSection(&m_condition_mutex);

		// Close the open event handles.
		CloseHandle(m_events[SIGNAL]);
		CloseHandle(m_events[BROADCAST]);
	}

	void ConditionVariable::Lock() noexcept {
		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_condition_mutex);
	}

	void ConditionVariable::Unlock() noexcept {
		// Release ownership of the specified critical section object.
		LeaveCriticalSection(&m_condition_mutex);
	}

	void ConditionVariable::Signal() noexcept {
		// Retrieve if there are waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		const bool has_waiters = (m_nb_waiters > 0);
		LeaveCriticalSection(&m_nb_waiters_mutex);

		if (has_waiters) {
			// Sets the SIGNAL event object to the signaled state.
			SetEvent(m_events[SIGNAL]);
		}
	}

	void ConditionVariable::Wait() noexcept {
		// Increase the number of waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		++m_nb_waiters;
		LeaveCriticalSection(&m_nb_waiters_mutex);

		// It is ok to release the <external_mutex> here since Win32
		// manual-reset events maintain state when used with <SetEvent>.  
		// This avoids the "lost wakeup" bug...
		LeaveCriticalSection(&m_condition_mutex);

		// Wait until one or all of the specified objects are 
		// in the signaled state (available for use) or the time-out interval elapses.
		// 1. The number of object handles.
		// 2. An array of object handles.
		// 3. The function returns when the state of any one object is signaled.
		// 4. The function will return only when the specified objects are signaled.
		const int result = WaitForMultipleObjects(2, m_events, FALSE, INFINITE);

		// Decrease the number of waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		--m_nb_waiters;
		// WAIT_OBJECT_0: The state of the specified object is signaled.
		const int last_waiter = (result == WAIT_OBJECT_0 + BROADCAST) && (m_nb_waiters == 0);
		LeaveCriticalSection(&m_nb_waiters_mutex);

		if (last_waiter) {
			// We are the last waiter to be notified or to stop waiting, so reset the manual event.
			// Sets the specified event object to the nonsignaled state.
			ResetEvent(m_events[BROADCAST]);
		}

		EnterCriticalSection(&m_condition_mutex);
	}
}