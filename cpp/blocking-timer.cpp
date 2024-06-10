#include <chrono>

void blocking_timer(const int interval_ms, const std::function<bool()> callback, const int thread_delay_ms = 10)
{
  std::chrono::time_point<std::chrono::system_clock> last_time;
  const std::chrono::milliseconds interval(interval_ms);
  const std::chrono::milliseconds delay(thread_delay_ms);

  while (true)
  {
    auto curr_time = std::chrono::system_clock::now();
    if (curr_time - last_time >= interval)
    {
      last_time = curr_time;

      if (!callback || !callback())
      {
        break;
      }
    }

    std::this_thread::sleep_for(delay);
  }
}