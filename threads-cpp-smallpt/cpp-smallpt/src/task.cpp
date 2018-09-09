#include "lock.hpp"
#include "task.hpp"

namespace smallpt {

	static HANDLE* s_threads;
	static Mutex s_s_task_queue_mutex;
	static std::vector< Task* > s_task_queue;
	static Semaphore* s_worker_semaphore;
	static size_t s_nb_unfinished_tasks;
	static ConditionVariable* s_tasks_running_condition;

	static DWORD WINAPI task_entry([[maybe_unused]] LPVOID lpParameter) {
		while (true) {
			s_worker_semaphore->Wait();

			Task* task = nullptr;
			{
				MutexLock lock(s_s_task_queue_mutex);
				if (s_task_queue.empty()) {
					break;
				}
				task = s_task_queue.back();
				s_task_queue.pop_back();
			}

			task->Run();

			s_tasks_running_condition->Lock();
			--s_nb_unfinished_tasks;
			if (0u == s_nb_unfinished_tasks) {
				s_tasks_running_condition->Signal();
			}
			s_tasks_running_condition->Unlock();
		}
		return 0;
	}

	void TasksInit() {
		static const std::size_t nb_s_threads = NumberOfSystemCores();
		s_worker_semaphore        = new Semaphore();
		s_tasks_running_condition = new ConditionVariable();

		s_threads = new HANDLE[nb_s_threads];
		for (std::size_t i = 0u; i < nb_s_threads; ++i) {
			s_threads[i] = CreateThread(nullptr, 0, task_entry, reinterpret_cast< void * >(i), 0, nullptr);
		}
	}

	void TasksCleanup() {
		if (!s_worker_semaphore) {
			return;
		}

		static const std::size_t nb_s_threads = NumberOfSystemCores();
		if (s_worker_semaphore) {
			s_worker_semaphore->Signal(static_cast<std::uint32_t >(nb_s_threads));
		}

		if (s_threads) {
			WaitForMultipleObjects(static_cast< DWORD >(nb_s_threads), s_threads, TRUE, INFINITE);

			for (std::size_t i = 0u; i < nb_s_threads; ++i) {
				CloseHandle(s_threads[i]);
			}

			delete[] s_threads;
			s_threads = nullptr;
		}
	}

	void EnqueueTasks(const std::vector< Task* >& tasks) {
		if (!s_threads) {
			TasksInit();
		}

		{
			MutexLock lock(s_s_task_queue_mutex);
			for (std::size_t i = 0u; i < tasks.size(); ++i) {
				s_task_queue.push_back(tasks[i]);
			}
		}

		s_tasks_running_condition->Lock();
		s_nb_unfinished_tasks += tasks.size();
		s_tasks_running_condition->Unlock();

		s_worker_semaphore->Signal(static_cast<std::uint32_t >(tasks.size()));
	}

	void WaitForAllTasks() {
		if (!s_tasks_running_condition) {
			return;
		}

		s_tasks_running_condition->Lock();
		while (0u < s_nb_unfinished_tasks) {
			s_tasks_running_condition->Wait();
		}
		s_tasks_running_condition->Unlock();
	}
};