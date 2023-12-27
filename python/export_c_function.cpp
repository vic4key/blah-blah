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
