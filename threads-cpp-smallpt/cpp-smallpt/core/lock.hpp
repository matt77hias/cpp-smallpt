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
		InitializeCriticalSection(&m_critical_section);
	}
	Mutex(Mutex &mutex);
	~Mutex() {
		DeleteCriticalSection(&m_critical_section);
	}

	CRITICAL_SECTION m_critical_section;
};

struct MutexLock final {

	MutexLock(Mutex &mutex) : m_mutex(mutex) {
		EnterCriticalSection(&m_mutex.m_critical_section);
	}
	~MutexLock() {
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
		InitializeCriticalSection(&m_nb_waiters_mutex);
		InitializeCriticalSection(&m_condition_mutex);

		m_events[SIGNAL] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		m_events[BROADCAST] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	}
	~ConditionVariable() { 
		DeleteCriticalSection(&m_nb_waiters_mutex);
		DeleteCriticalSection(&m_condition_mutex);
		
		CloseHandle(m_events[SIGNAL]);
		CloseHandle(m_events[BROADCAST]);
	}

	void Lock() {
		EnterCriticalSection(&m_condition_mutex);
	}
	void Unlock() {
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
