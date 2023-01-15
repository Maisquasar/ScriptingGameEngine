#include "../Source/includes/Core/DataStructure/ThreadManager.hpp"

ThreadManager::ThreadManager() {}
ThreadManager::~ThreadManager()
{
	for (int i = 0; i < this->Threads.size(); i++)
		Threads[i].join();
}

void ThreadManager::Start()
{
	_threadsAvaible = std::thread::hardware_concurrency();
	Threads.resize(_threadsAvaible);
	for (uint32_t i = 0; i < _threadsAvaible; i++) {
		Threads.at(i) = std::thread(&ThreadManager::ThreadLoop, this);
	}
	Done.clear();
}

void ThreadManager::ThreadLoop()
{
	while (!_terminate)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!Tasks.empty())
		{
			this->Lock();
			if (!Tasks.empty()) {
				auto task = Tasks.front();
				Tasks.pop();
				this->Unlock();
				if (task != nullptr)
					task();
				break;
			}
			this->Unlock();
		}
	}
	if (_terminate)
	{
		return;
	}
	ThreadLoop();
}

void ThreadManager::DeleteThreads()
{
	_terminate = true;
	for (int i = 0; i < Threads.size(); i++)
		Threads[i].join();
	Threads.clear();
	Done.clear();
}

void ThreadManager::Lock()
{
	while (Done.test_and_set()) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
}
void ThreadManager::Unlock()
{
	Done.clear();
}