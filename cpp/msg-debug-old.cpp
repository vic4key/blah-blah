#include <windows.h>
#include <memory>

void msg_debug_A(const std::string format, ...)
{
  va_list args;
  va_start(args, format);

  int N = _vscprintf(format.c_str(), args) + 1;
  std::unique_ptr<char[]> p(new char[N]);
  ZeroMemory(p.get(), N);

  vsnprintf(p.get(), N, format.c_str(), args);
  OutputDebugStringA(p.get());

  va_end(args);
}

void msg_debug_W(const std::wstring format, ...)
{
  va_list args;
  va_start(args, format);

  int N = _vscwprintf(format.c_str(), args) + 1;
  std::unique_ptr<wchar_t[]> p(new wchar_t[N]);
  ZeroMemory(p.get(), N);

  vswprintf(p.get(), N, format.c_str(), args);
  OutputDebugStringW(p.get());

  va_end(args);
}

#ifdef _UNICODE
#define ts(x) L ## x
#define msg_debug msg_debug_W
#else
#define ts(x) x
#define msg_debug msg_debug_A
#endif // _UNICODE
