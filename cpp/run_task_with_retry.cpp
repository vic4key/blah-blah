// -fext-numeric-literals --std=c++17

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
using namespace std;

bool run_task_with_retry(
    const std::function<bool()> task,
    const unsigned retry_count = 1, const unsigned retry_duration_ms = 1000)
{
    if (task == nullptr)
    {
        return false;
    }

    if (task())
    {
        return true;
    }

    bool result = false;

    for (unsigned i = 0; i < retry_count; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(retry_duration_ms));

        if (task())
        {
            result = true;
            break;
        }
    }

    return result;
}

bool try_run_task(
    const std::function<bool()> task,
    const unsigned try_count = 1, const unsigned try_duration_ms = 1000)
{
    if (task == nullptr || try_count == 0)
    {
        return false;
    }

    bool result = false;

    for (unsigned i = 0; i < try_count; i++)
    {
        if (i > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(try_duration_ms));
        }

        if (task())
        {
            result = true;
            break;
        }
    }

    return result;
}

int main()
{
    printf("BEGIN\n");

    auto start_time = std::chrono::high_resolution_clock::now();
    
    int count = 0;

    run_task_with_retry([&]() -> bool
    {
        std::chrono::duration<double, std::milli> delta_time = std::chrono::high_resolution_clock::now() - start_time;
        printf("%s => %.1fs\n", __PRETTY_FUNCTION__, delta_time / 1000);
        return count++ == 10;
    }, 3);

    std::chrono::duration<double, std::milli> delta_time = std::chrono::high_resolution_clock::now() - start_time;
    printf("END %.1f s\n", delta_time / 1000);

    return 0;

    return 0;
}