#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>

struct Task
{
	std::function<void()> function;
	std::string nameTask;
};

class ThreadPool
{
private:
	std::vector<std::thread> threads;
	std::queue<Task> tasks;
	std::condition_variable cv;
	std::atomic<bool> workingPool = true;
	std::mutex mtx;

public:
	explicit ThreadPool(size_t numThreads)
	{
		for (size_t i = 0; i < numThreads; ++i)
		{
			threads.emplace_back([this]() {
				while (workingPool)
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this]() {return !tasks.empty() || workingPool == false; });
					if (!workingPool && tasks.empty()) break;
					Task current = tasks.front();
					tasks.pop();
					lock.unlock();
					current.function();
				}
				});
		}
	}

	void addTask(std::function<void()> task, const std::string& nameTask)
	{
		std::lock_guard<std::mutex> lock(mtx);
		Task currentTask = { task, nameTask };
		tasks.push(currentTask);
		cv.notify_one();
	}

	~ThreadPool()
	{
		{
			std::lock_guard<std::mutex> lock(mtx);
			workingPool = false;
		}
		cv.notify_all();
		for (std::thread& t : threads)
		{
			t.join();
		}
	}
};
