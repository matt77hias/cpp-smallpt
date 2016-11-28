#pragma once

class Mutex final {

friend struct MutexLock;

public:

	static Mutex *Create() {
		return new Mutex();
	}

	static void Destroy(Mutex *mutex) {
		delete mutex;
	}

private:

	Mutex() {
		// Initialize a critical section object.
		InitializeCriticalSection(&m_critical_section);
	}
	Mutex(Mutex &mutex);
	~Mutex() {
		// Release all resources used by an unowned critical section object. 
		DeleteCriticalSection(&m_critical_section);
	}

	CRITICAL_SECTION m_critical_section;
};

struct MutexLock final {

	MutexLock(Mutex &mutex) : m_mutex(mutex) {
		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_mutex.m_critical_section);
	}
	~MutexLock() {
		// Release ownership of the specified critical section object.
		LeaveCriticalSection(&m_mutex.m_critical_section);
	}

private:

	MutexLock(const MutexLock &mutex_lock);

	Mutex &m_mutex;
};

class Semaphore final {

public:

	Semaphore();
	~Semaphore() {
		// Closes an open object handle.
		CloseHandle(m_handle);
	}

	void Post(uint32_t count = 1);
	void Wait();
	bool TryWait();

private:

	HANDLE m_handle;
};

class ConditionVariable final {

public:

	ConditionVariable() : m_nb_waiters(0) {
		// Initialize the critical section objects
		// for the number of waiters and condition.
		InitializeCriticalSection(&m_nb_waiters_mutex);
		InitializeCriticalSection(&m_condition_mutex);

		// Creates or opens a named or unnamed event object.
		// On success, a handle to the event object is returned.
		m_events[SIGNAL] = CreateEvent(
								nullptr,  // no security
								FALSE, // auto-reset event object
								FALSE, // non-signaled initial state
								nullptr); // unnamed event object
		m_events[BROADCAST] = CreateEvent(
								nullptr,  // no security
								TRUE,  // manual-reset event object
								FALSE, // non-signaled initial state
								nullptr); // unnamed event object
	}
	~ConditionVariable() {
		// Release all resources used by an unowned critical section object. 
		DeleteCriticalSection(&m_nb_waiters_mutex);
		DeleteCriticalSection(&m_condition_mutex);

		// Close the open event handles.
		CloseHandle(m_events[SIGNAL]);
		CloseHandle(m_events[BROADCAST]);
	}

	void Lock() {
		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_condition_mutex);
	}
	void Unlock() {
		// Release ownership of the specified critical section object.
		LeaveCriticalSection(&m_condition_mutex);
	}

	void Wait();
	void Signal();

private:

	uint32_t m_nb_waiters;
	CRITICAL_SECTION m_nb_waiters_mutex;
	CRITICAL_SECTION m_condition_mutex;
	enum { SIGNAL = 0, BROADCAST = 1, NUM_EVENTS = 2 };
	HANDLE m_events[NUM_EVENTS];
};