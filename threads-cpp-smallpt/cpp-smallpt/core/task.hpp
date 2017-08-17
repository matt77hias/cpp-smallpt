#pragma once

#include <stdint.h>
#include <vector>

#include "windows.hpp"

namespace smallpt {

	inline size_t NumberOfSystemCores() noexcept {
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		return static_cast<size_t>(system_info.dwNumberOfProcessors);
	}

	class Task {

	public:

		Task() = default;
		Task(const Task &task) = default;
		Task(Task &&task) = default;
		virtual ~Task() = default;

		Task &operator=(const Task &task) = default;
		Task &operator=(Task &&task) = default;

		virtual void Run() noexcept = 0;
	};

	void TasksInit();
	void TasksCleanup();

	void EnqueueTasks(const std::vector< Task * > &tasks);
	void WaitForAllTasks();

}