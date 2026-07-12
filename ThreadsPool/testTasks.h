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

	while (count > 0)
	{
		sum = sum + distrib(gen);
		--count;
	}
	std::cout << "Sum of " << count << " random numbers: " << " " << std::endl;
}

std::vector<std::string> sumVectors(std::vector<std::string> names, std::vector<std::string> surnames,
	std::vector<std::string> ages, int count)
{
	std::vector<std::string> result;
	result.reserve(count);
	for (size_t i = 0; i < count; ++i)
	{
		result.push_back(names[i] + " " + surnames[i] + " " + ages[i]);
	}
	return result;
}
