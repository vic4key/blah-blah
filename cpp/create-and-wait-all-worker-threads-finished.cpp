// main thread creates many worker threads, then waiting for they all finished
// all worker threads will notify to main thread (each thread notify once when it finished its job)
// when all worker thread finished their jobs, main thread stop waiting and run continue

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;

#include "_cout.hpp"

int main()
{
  using clock_t = std::chrono::high_resolution_clock;
  using duration_t = std::chrono::duration<double>;
  auto t = clock_t::now();

  const int num_thread = std::thread::hardware_concurrency();
  std::atomic<int> thread_count = 0;
  std::condition_variable cv;

  // create many worker threads (independency)
  for (int i = 0; i < num_thread; ++i)
  {
    std::thread([&](int i) -> void
    {
      vu::_cout_A("worker_thread #", i, " started\n");
      std::this_thread::sleep_for(seconds(i + 1));
      vu::_cout_W(L"worker_thread #", i, L" finished\n");

      thread_count += 1;
      cv.notify_one();
    }, i).detach(); // detach the worker thread from main thread to don't wait it running
  }

  // main thread waiting for all worker threads are finished
  std::mutex mtx;
  std::unique_lock<std::mutex> lk(mtx);
  cv.wait(lk, [&]() -> bool
  {
    return thread_count == num_thread;
  });

  duration_t d = clock_t::now() - t;
  vu::_cout("main_thead waited ", d.count(), " seconds\n");

  return 0;
}