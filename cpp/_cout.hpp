// C++14 (MSVC 2015+ or MinGW 5.1+)
#if (defined(_MSC_VER) && _MSVC_LANG >= 201402L) || (defined(__MINGW32__) && __cplusplus >= 201402L)

#include <sstream>
#include <iostream>

namespace vu
{

template<typename Stream>
void __vu_cout_impl(Stream& stream) {}

template<typename Stream, typename T, typename... Args>
void __vu_cout_impl(Stream& stream, T&& t, Args&&... args)
{
  stream << std::forward<T>(t);
  __vu_cout_impl(stream, std::forward<Args>(args)...);
}

template<typename... Args>
void _cout_A(Args&&... args)
{
  std::stringstream ss;
  __vu_cout_impl(ss, std::forward<Args>(args)...);
  std::cout << ss.str();
}

template<typename... Args>
void _cout_W(Args&&... args)
{
  std::wstringstream ss;
  __vu_cout_impl(ss, std::forward<Args>(args)...);
  std::wcout << ss.str();
}

#ifdef _UNICODE
#define _cout _cout_W
#else
#define _cout _cout_A
#endif

} // vu

#endif // C++14 (MSVC 2015+ or MinGW 5.1+)