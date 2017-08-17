#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdint.h>

#include "windows.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	/**
	 A struct of mutexes.
	 */
	struct Mutex final {

	friend struct MutexLock;

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a mutex.
		 */
		Mutex();

		/**
		 Constructs a mutex from the given mutex.

		 @param[in]		mutex
						A reference to the mutex to copy.
		 */
		Mutex(const Mutex &mutex) = delete;

		/**
		 Constructs a mutex by moving the given mutex.

		 @param[in]		mutex
						A reference to the mutex to move.
		 */
		Mutex(Mutex &&mutex) = default;

		/**
		 Destructs this mutex.
		 */
		~Mutex();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given mutex to this mutex.

		 @param[in]		mutex
						A reference to the mutex to copy.
		 @return		A reference to the copy of the given mutex 
						(i.e. this mutex).
		 */
		Mutex &operator=(const Mutex &mutex) = delete;

		/**
		 Moves the given mutex to this mutex.

		 @param[in]		mutex
						A reference to the mutex to move.
		 @return		A reference to the moved mutex
						(i.e. this mutex).
		 */
		Mutex &operator=(Mutex &&mutex) = delete;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The critical section object of this mutex.
		 */
		CRITICAL_SECTION m_critical_section;
	};

	/**
	 A struct of mutex locks.
	 */
	struct MutexLock final {

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a mutex lock for the given mutex.

		 @param[in]		mutex
						A reference to the mutex.
		 */
		explicit MutexLock(Mutex &mutex);

		/**
		 Constructs a mutex lock from the given mutex lock.

		 @param[in]		mutex_lock
						A reference to the mutex lock to copy.
		 */
		MutexLock(const MutexLock &mutex_lock) = delete;

		/**
		 Constructs a mutex lock by moving the given mutex lock.

		 @param[in]		mutex_lock
						A reference to the mutex lock to move.
		 */
		MutexLock(MutexLock &&mutex_lock) = default;

		/**
		 Destructs this mutex lock.
		 */
		~MutexLock();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given mutex lock to this mutex lock.

		 @param[in]		mutex_lock
						A reference to the mutex lock to copy.
		 @return		A reference to the copy of the given mutex lock 
						(i.e. this mutex lock)
		 */
		MutexLock &operator=(const MutexLock &mutex_lock) = delete;

		/**
		 Moves the given mutex lock to this mutex lock.

		 @param[in]		mutex_lock
						A reference to the mutex lock to move.
		 @return		A reference to the moved mutex lock
						(i.e. this mutex lock)
		 */
		MutexLock &operator=(MutexLock &&mutex_lock) = delete;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 A reference to the mutex of this mutex lock.
		 */
		Mutex &m_mutex;
	};

	/**
	 A struct of semaphores.
	 */
	struct Semaphore final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a semaphore.
		 */
		Semaphore();

		/**
		 Constructs a semaphore from the given semaphore.

		 @param[in]		semaphore
						A reference to the semaphore to copy.
		 */
		Semaphore(const Semaphore &semaphore) = delete;

		/**
		 Constructs a semaphore by moving the given semaphore.

		 @param[in]		semaphore
						A reference to the semaphore to move.
		 */
		Semaphore(Semaphore &&semaphore) = default;

		/**
		 Destructs this semaphore.
		 */
		~Semaphore();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given semaphore to this semaphore.

		 @param[in]		semaphore
						A reference to the semaphore to copy.
		 @return		A reference to the copy of the given semaphore
						(i.e. this semaphore)
		 */
		Semaphore &operator=(const Semaphore &semaphore) = delete;

		/**
		 Copies the given semaphore to this semaphore.

		 @param[in]		semaphore
						A reference to the semaphore to move.
		 @return		A reference to the moved semaphore
						(i.e. this semaphore)
		 */
		Semaphore &operator=(Semaphore &&semaphore) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Increments the value of this semaphore variable by the given value.
		 
		 If the initial value of the semaphore is negative, the waiting queue is not
		 empty and thus one blocked process can be transferred to the ready queue.

		 @param[in]		count
						The increment value.
		 */
		void Signal(uint32_t count = 1) noexcept;
		
		/**
		 Decrements the value of this semaphore variable by one.

		 The process executing wait is blocked until the value of the semaphore
		 is greater or equal to 1.
		 */
		void Wait() noexcept;
		
		/**
		 Checks whether waiting for this semaphore would be necessary.

		 @return		@c true if waiting for this semaphore would be necessary.
						@c false otherwise.
		 */
		bool TryWait() noexcept;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The handle of this semaphore.
		 */
		HANDLE m_handle;
	};

	/**
	 A struct of condition variables.
	 */
	struct ConditionVariable final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a condition variable.
		 */
		ConditionVariable();

		/**
		 Constructs a condition variable from the given condition variable.

		 @param[in]		condition_variable
						A reference to the condition variable to copy.
		 */
		ConditionVariable(const ConditionVariable &condition_variable) = delete;

		/**
		 Constructs a condition variable by moving the given condition variable.

		 @param[in]		condition_variable
						A reference to the condition variable to move.
		 */
		ConditionVariable(ConditionVariable &&condition_variable) = default;

		/**
		 Destructs this condition variable.
		 */
		~ConditionVariable();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given condition variable to this condition variable.

		 @param[in]		condition_variable
						A reference to the condition variable to copy.
		 @return		A reference to the copy of the given condition variable
						(i.e. this condition variable)
		 */
		ConditionVariable &operator=(const ConditionVariable &condition_variable) = delete;

		/**
		 Moves the given condition variable to this condition variable.

		 @param[in]		condition_variable
						A reference to the condition variable to move.
		 @return		A reference to the moved condition variable
						(i.e. this condition variable)
		 */
		ConditionVariable &operator=(ConditionVariable &&condition_variable) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Locks this condition variable.
		 */
		void Lock() noexcept;

		/**
		 Unlocks this condition variable.
		 */
		void Unlock() noexcept;

		/**
		 Signals a condition change.
		 */
		void Signal() noexcept;

		/**
		 Waits for a signal indicating a condition change.
		 */
		void Wait() noexcept;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/** 
		 The number of waiters of this condition variable.
		 */
		uint32_t m_nb_waiters;

		/**
		 The critical section object for the mutex guarding @c m_nb_waiters
		 of this condition variable.
		 */
		CRITICAL_SECTION m_nb_waiters_mutex;

		/**
		 The critical section object for the mutex guarding the condition
		 of this condition variable.
		 */
		CRITICAL_SECTION m_condition_mutex;

		/**
		 An enumeration of the different types of events of this condition variable.
		 */
		enum Event { 
			SIGNAL    = 0, 
			BROADCAST = 1, 
			COUNT     = 2 
		};

		/**
		 The signal and broadcast event handles of this condition variable.
		 */
		HANDLE m_events[COUNT];
	};
}