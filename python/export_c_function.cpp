#include <stdio.h>

#pragma optimize("", off)

extern "C" __declspec(dllexport) void change_ptr_var(int* pvar)
{
  if (pvar != nullptr)
  {
    *pvar = 123;
  }
}

extern "C" __declspec(dllexport) void change_ref_var(int& var)
{
  var = 456;
}

extern "C" __declspec(dllexport) void print_message(const char* message)
{
  printf("%s\n", message);
}

extern "C" __declspec(dllexport) void c_invoke_print_message()
{
  print_message("This is a string from C code");
}

#pragma optimize("", on)