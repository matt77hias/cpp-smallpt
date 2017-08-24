#include "stdafx.h"

#include "lock.hpp"

namespace smallpt {

	static HANDLE *threads;
	static Mutex task_queue_mutex;
	static std::vector< Task * > task_queue;
	static Semaphore *worker_semaphore;
	static size_t nb_unfinished_tasks;
	static ConditionVariable *tasks_running_condition;

	static DWORD WINAPI task_entry(LPVOID lpParameter) {
		(void)lpParameter;

		while (true) {
			worker_semaphore->Wait();

			Task *task = nullptr;
			{
				MutexLock lock(task_queue_mutex);
				if (task_queue.size() == 0) {
					break;
				}
				task = task_queue.back();
				task_queue.pop_back();
			}

			task->Run();

			tasks_running_condition->Lock();
			--nb_unfinished_tasks;
			if (nb_unfinished_tasks == 0) {
				tasks_running_condition->Signal();
			}
			tasks_running_condition->Unlock();
		}
		return 0;
	}

	void TasksInit() {
		static const size_t nb_threads = NumberOfSystemCores();
		worker_semaphore        = new Semaphore();
		tasks_running_condition = new ConditionVariable();

		threads = new HANDLE[nb_threads];
		for (size_t i = 0; i < nb_threads; ++i) {
			threads[i] = CreateThread(nullptr, 0, task_entry, reinterpret_cast< void * >(i), 0, nullptr);
		}
	}

	void TasksCleanup() {
		if (!worker_semaphore) {
			return;
		}

		static const size_t nb_threads = NumberOfSystemCores();
		if (worker_semaphore) {
			worker_semaphore->Signal(static_cast< uint32_t >(nb_threads));
		}

		if (threads) {
			WaitForMultipleObjects(static_cast< DWORD >(nb_threads), threads, TRUE, INFINITE);

			for (size_t i = 0; i < nb_threads; ++i) {
				CloseHandle(threads[i]);
			}

			delete[] threads;
			threads = nullptr;
		}
	}

	void EnqueueTasks(const std::vector<Task *> &tasks) {
		if (!threads) {
			TasksInit();
		}

		{
			MutexLock lock(task_queue_mutex);
			for (size_t i = 0; i < tasks.size(); ++i) {
				task_queue.push_back(tasks[i]);
			}
		}

		tasks_running_condition->Lock();
		nb_unfinished_tasks += tasks.size();
		tasks_running_condition->Unlock();

		worker_semaphore->Signal(static_cast< uint32_t >(tasks.size()));
	}

	void WaitForAllTasks() {
		if (!tasks_running_condition) {
			return;
		}

		tasks_running_condition->Lock();
		while (nb_unfinished_tasks > 0) {
			tasks_running_condition->Wait();
		}
		tasks_running_condition->Unlock();
	}
};