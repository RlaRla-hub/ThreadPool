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

	std::vector<bool> stateIsIdle;        
	std::vector<std::string> namesTask;

public:
	explicit ThreadPool(size_t numThreads)
	{
		stateIsIdle.resize(numThreads, true);
		namesTask.resize(numThreads, " ");

		for (size_t i = 0; i < numThreads; ++i)
		{
			threads.emplace_back([this, i]() {
				while (workingPool)
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this]() {return !tasks.empty() || workingPool == false; });
					if (!workingPool && tasks.empty()) break;
					Task current = tasks.front();
					tasks.pop();

					stateIsIdle[i] = false;
					namesTask[i] = current.nameTask;

					lock.unlock();       
					current.function();     
					lock.lock();        

					stateIsIdle[i] = true;
					namesTask[i] = " ";
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

	void getThreadPoolInfo()
	{
		std::lock_guard<std::mutex> lock(mtx);

		for (size_t i = 0; i < threads.size(); ++i)
		{
			std::cout << "Thread: " << i << ", threadID: " << threads[i].get_id();
			std::cout << "State of thred: << " << stateIsIdle[i] << ", ";
			std::cout << "Name of thred: << " << namesTask[i] << " ";
			std::cout << " " << std::endl;
			std::cout << std::endl;
		}
	}


	~ThreadPool()
	{
	    workingPool = false;
		cv.notify_all();
		for (std::thread& t : threads)
		{
			t.join();
		}
	}
};
