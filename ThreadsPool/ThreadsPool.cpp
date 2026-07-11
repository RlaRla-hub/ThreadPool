#include <iostream>
#include "ThreadsPool.h"
#include "testTasks.h"

int main()
{
    ThreadPool pool(10);
    pool.addTask([]() { sleepTask(200); }, "sleepTask");
}
