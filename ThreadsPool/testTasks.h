#pragma once


#include <iostream>
#include <random>
#include <thread>

inline std::mutex coutMtx;

void sleepTask(int numSeconds) 
{
	std::unique_lock<std::mutex> lock(coutMtx);
	std::cout << "Sleeping " << numSeconds << " seconds" << std::endl;
	lock.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(numSeconds));
	lock.lock();
	std::cout << "Get up" << std::endl;
	lock.unlock();
}


void sumRandomNumbers(size_t count)
{
	int sum = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);

	if (count > 10000000)
	{
		std::unique_lock<std::mutex> lock(coutMtx);
		std::cout << "Invaide count" << std::endl;
	}
	else
	{
		for (size_t i = count; i > 0; --i)
		{
			sum = sum + distrib(gen);
		}

		std::unique_lock<std::mutex> lock(coutMtx);
		std::cout << "Sum of " << count << " random numbers: " << sum << std::endl;
	}
}

std::vector<std::string> sumVectors(std::vector<std::string> names, std::vector<std::string> surnames,
	std::vector<std::string> ages, int count)
{
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::vector<std::string> result;
	result.reserve(count);
	for (size_t i = 0; i < count; ++i)
	{
		result.push_back(names[i] + " " + surnames[i] + " " + ages[i]);
	}
	return result;
}
