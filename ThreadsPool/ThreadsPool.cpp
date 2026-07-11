#include <iostream>
#include "ThreadsPool.h"
#include "testTasks.h"

int main()
{
    ThreadPool pool(10);
    pool.addTask([]() { sleepTask(100); }, "sleepTask");
    pool.addTask([]() { sleepTask(60); }, "sleepTask");
    pool.addTask([]() { sleepTask(10); }, "sleepTask");
    pool.addTask([]() { sleepTask(20); }, "sleepTask");
    pool.addTask([]() { sleepTask(5); }, "sleepTask");

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  

    pool.getThreadPoolInfo();
}
