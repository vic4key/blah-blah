#include <chrono>
#include <cstdio>
#include <windows.h>

#define CHRONO_LOGGER_START()\
  auto blabla_time_start = std::chrono::steady_clock::now();

#define CHRONO_LOGGER_STOP(text)\
  {\
    auto blabla_time_stop = std::chrono::steady_clock::now();\
    auto blabla_time_elapsed = std::chrono::duration<double>(blabla_time_stop - blabla_time_start);\
    char s[1024] = { 0 };\
    memset(s, 0, sizeof(s));\
    sprintf_s(s, sizeof(s), "%s %.3f", text, blabla_time_elapsed.count());\
    OutputDebugStringA(s);\
  }
