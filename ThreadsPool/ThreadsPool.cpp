#include <iostream>
#include "ThreadsPool.h"
#include "testTasks.h"
#include <cstdlib>
#include <string>

int main()
{
    ThreadPool pool(10);

    std::cout << "List of keys: " << std::endl;
    std::cout << "1 - sleepTask " << std::endl;
    std::cout << "2 - sumRandomNumbers " << std::endl;
    std::cout << "3 - sumVectors " << std::endl;
    std::cout << "4 - get info about Thread Pool " << std::endl;
    std::cout << "5 - exit " << std::endl;

    std::vector<std::string> names = { "Margo", "Vika", "Maria", "Elena", "Renata" };
    std::vector<std::string> surnames = { "Oduvanchikova", "Ivanova", "Stepanova", "Bukova", "Travova" };
    std::vector<std::string> ages = { "23", "52", "67", "88", "3" };

    bool getOutLoop = true;

    while (getOutLoop)
    {
        std::string keyString;
        std::cin >> keyString;

        bool isNumber = true;
        for (char c : keyString) {
            if (!std::isdigit(c)) {
                isNumber = false;
                std::cout << "Invalid key - not number " << std::endl;
                break;
            }
        }

        if (!isNumber) {
            break;
        }

        size_t key = std::stoull(keyString);

        switch (key)
        {
            case 1: 
            {
                std::cout << "Enter quntity seconds: ";
                int seconds;
                std::cin >> seconds;
                pool.addTask([seconds]() { sleepTask(seconds); }, "sleepTask");
                break;
            }
            case 2:
            {
                std::cout << "Enter quntity summands (max 10 000 000): ";
                std::string summandsString;
                std::cin >> summandsString;

                bool isNumber = true;
                for (char c : summandsString) {
                    if (!std::isdigit(c)) {
                        isNumber = false;
                        std::cout << "Invalid input - not number " << std::endl;
                        break;
                    }
                }

                if (!isNumber) {
                    break;
                }

                if (summandsString.length() > 10) {
                    std::cout << "Number too large - max 10 000 000" << std::endl;
                    break;
                }

                size_t summands = std::stoull(summandsString);

                pool.addTask([summands]() { sumRandomNumbers(summands); }, "sumRandomNumbers");
                break;
            }
            case 3:
            {
                std::future<std::vector<std::string>> futureVector = pool.addTaskWithResult<std::vector<std::string>>(
                    [names, surnames, ages]() {
                        return sumVectors(names, surnames, ages, 5);
                    },
                    "sumVectors"
                );
                std::vector<std::string> result = futureVector.get();
                for (const auto& str : result) {
                    std::cout << str << std::endl;
                }
                break;
            }
            case 4:
            {
                pool.getThreadPoolInfo();
                break;
            }
            case 5:
            {
                getOutLoop = false;
                break;
            }
            default:
            {
                std::cout << "Invalid key" << std::endl;
            }
        }
    }
}
