#include "stdafx.h"

Semaphore::Semaphore() {
	m_handle = CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);
}

void Semaphore::Post(uint32_t count) {
	ReleaseSemaphore(m_handle, count, nullptr);
}

void Semaphore::Wait() {
	WaitForSingleObject(m_handle, INFINITE);
}

bool Semaphore::TryWait() {
	return (WaitForSingleObject(m_handle, 0L) == WAIT_OBJECT_0);
}

void ConditionVariable::Wait() {
	EnterCriticalSection(&m_nb_waiters_mutex);
	++m_nb_waiters;
	LeaveCriticalSection(&m_nb_waiters_mutex);

	LeaveCriticalSection(&m_condition_mutex);

	const int result = WaitForMultipleObjects(2, m_events, FALSE, INFINITE);

	EnterCriticalSection(&m_nb_waiters_mutex);
	--m_nb_waiters;
	const int last_waiter = (result == WAIT_OBJECT_0 + BROADCAST) && (m_nb_waiters == 0);
	LeaveCriticalSection(&m_nb_waiters_mutex);

	if (last_waiter) {
		ResetEvent(m_events[BROADCAST]);
	}

	EnterCriticalSection(&m_condition_mutex);
}

void ConditionVariable::Signal() {
	EnterCriticalSection(&m_nb_waiters_mutex);
	const bool has_waiters = (m_nb_waiters > 0);
	LeaveCriticalSection(&m_nb_waiters_mutex);

	if (has_waiters) {
		SetEvent(m_events[SIGNAL]);
	}
}