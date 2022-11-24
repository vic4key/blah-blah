#include <chrono>
#include <cstdio>
#include <windows.h>

#ifndef   BLABLA_CTM_2
auto blabla_time_start = std::chrono::steady_clock::now();
#else  // BLABLA_CTM_2
extern std::chrono::steady_clock::time_point blabla_time_start;
#endif // BLABLA_CTM_2

#define CHRONO_LOGGER_START()\
  blabla_time_start = std::chrono::steady_clock::now();

#define CHRONO_LOGGER_STOP(text)\
  {\
    auto blabla_time_stop = std::chrono::steady_clock::now();\
    auto blabla_time_elapsed = std::chrono::duration<double>(blabla_time_stop - blabla_time_start);\
    char s[1024] = { 0 };\
    memset(s, 0, sizeof(s));\
    sprintf_s(s, sizeof(s), "%s %.3f", text, blabla_time_elapsed.count());\
    OutputDebugStringA(s);\
  }
