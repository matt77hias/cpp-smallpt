#pragma once

inline uint32_t NumberOfSystemCores() {
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	return system_info.dwNumberOfProcessors;
}

class Task {

public:

	virtual ~Task() {}

	virtual void Run() = 0;
};

void TasksInit();
void TasksCleanup();

void EnqueueTasks(const std::vector<Task *> &tasks);
void WaitForAllTasks();