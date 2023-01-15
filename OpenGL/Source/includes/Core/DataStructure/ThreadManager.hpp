#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <queue>

static std::atomic_flag Done;
class ThreadManager
{
public:
	std::vector<std::thread> Threads;
	std::queue<std::function<void()>> Tasks;
	ThreadManager();
	~ThreadManager();

	void Start();
	void ThreadLoop();
	static void Lock();
	static void Unlock();
	template <typename F, typename... A> void QueueJob(F&& task, A&&... args)
	{
		std::function<void()> task_function = std::bind(std::forward<F>(task), std::forward<A>(args)...);
		{
			Tasks.push(task_function);
		}
		++this->_totalTasks;
	}
	void DeleteThreads();
	bool _terminate = false;
private:
	std::atomic<int> _totalTasks = 0;
	int _threadsAvaible;

};