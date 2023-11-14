// main thread creates many worker threads, but they are waiting for notify from main thread
// when main thread notifies, all worker threads are start running
// when all worker thread finished their jobs, main thread stop waiting and run continue

#include <vector>
#include <thread>
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
  bool run_all_worker_threads = false;
  std::condition_variable cv;
  std::mutex mtx;

  // create many worker threads
  std::vector<std::thread> threads;
  for (int i = 0; i < num_thread; ++i)
  {
    threads.push_back(std::thread([&](int i) -> void
    {
      vu::_cout_A("worker thread #", i, " started\n");

      std::unique_lock<std::mutex> lk(mtx);
      cv.wait(lk, [&]
      {
        return run_all_worker_threads;
      });

      vu::_cout_W(L"worker thread #", i, L" finished\n");
    }, i));
  }

  // main thread sleep for a short period to demonstrate all threads are the blocking
  const int num_waiting_seconds = 3;
  for (int i = 0; i < num_waiting_seconds; i++)
  {
    std::this_thread::sleep_for(1s);
    vu::_cout("all worker threads will run in ", num_waiting_seconds - i, " seconds\n");
  }

  // signal the condition variable to unblock all waiting threads
  vu::_cout("main thread notifies all worker threads to run\n");
  {
    std::lock_guard<std::mutex> lock(mtx);
    run_all_worker_threads = true;
    cv.notify_all();
  }

  vu::_cout("all worker threads are running\n");
  for (auto& t : threads) t.join(); // waiting all threads are finished
  vu::_cout("all worker threads are finished\n");

  duration_t d = clock_t::now() - t;
  vu::_cout("main thead waited ", d.count(), " seconds\n");

  return 0;
}