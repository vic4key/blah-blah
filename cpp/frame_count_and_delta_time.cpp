void this_function_is_calling_every_times()
{
    // these codes are counting the number of calls per second
    // (đếm số lần hàm này được gọi trong khoảng thời gian mỗi giây)
    {
        static int fps_count = 0;
        fps_count += 1;
        static std::chrono::steady_clock::time_point time = std::chrono::high_resolution_clock::now();
        std::chrono::steady_clock::time_point temp = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = temp - time;
        if (duration.count() >= 1000)
        {
            ALOGI("[vic] %s : fps_count = %d", __PRETTY_FUNCTION__, fps_count);
            fps_count = 0;
            time = temp;
        }
    }

    // the function body
}

void this_function_is_calling_every_times()
{
    // these codes limits the logging frequency to not exceed approximately 30 times per second
    // (giới hạn tần suất ghi log sao cho không vượt quá khoảng 30 lần mỗi giây)
    {
        static const int64_t fps = 30;
        static const int64_t fps_interval_ms = int64_t(1000.F / fps);
        static auto prev_time = std::chrono::high_resolution_clock::now();
        auto curr_time = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - prev_time).count();
        if (duration_ms >= fps_interval_ms)
        {
            prev_time = curr_time;
            ALOGI("[vic] %s : duration_ms = %d ms", __PRETTY_FUNCTION__, duration_ms);
        }
    }

    // the function body
}
