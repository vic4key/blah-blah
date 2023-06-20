#include <iostream>
#include <string>
#include <utility>
#include <typeinfo>

void overloaded_function(std::string& s)
{
  std::cout << "std::string&  : " << s << std::endl;
}

void overloaded_function(std::string&& s)
{
  std::cout << "std::string&& : " << s << std::endl;
}

template<typename T>
void call_indirect_via_std_forward(T&& params)
{
  overloaded_function(std::forward<T>(params));
}

int main()
{
  std::string test = "this is a string";
  call_indirect_via_std_forward(test);
  call_indirect_via_std_forward(std::move(test));
}