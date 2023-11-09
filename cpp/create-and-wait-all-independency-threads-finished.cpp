#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std::chrono_literals;

int main()
{
  using clock_t = std::chrono::high_resolution_clock;
  using duration_t = std::chrono::duration<double>;
  auto t = clock_t::now();

  std::condition_variable cv;

  const int num_thread = 7;
  std::atomic<int> thread_count = 0;

  // create many independency threads

  for (int i = 0; i < num_thread; ++i)
  {
    std::thread([&]() -> void
    {
      std::cout << "thread " << std::this_thread::get_id() << " started\n";

      int i = 0;
      while (i++ < 5) // each thread run in 5s
      {
        std::this_thread::sleep_for(1s);
      }

      std::cout << "thread " << std::this_thread::get_id() << " finished\n";

      thread_count += 1;
      cv.notify_one();
    }).detach();
  }

  // waiting for all independency threads are finished

  std::mutex mtx;
  std::unique_lock<std::mutex> lk(mtx);
  cv.wait(lk, [&]() -> bool
  {
    return thread_count == num_thread;
  });

  duration_t d = clock_t::now() - t;
  std::cout << "time = " << d.count() << " seconds" << std::endl;

  return 0;
}