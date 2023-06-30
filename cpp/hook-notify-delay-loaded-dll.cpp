// https://learn.microsoft.com/en-us/cpp/build/reference/error-handling-and-notification
// https://learn.microsoft.com/en-us/cpp/build/reference/understanding-the-helper-function

#ifndef DELAYIMP_INSECURE_WRITABLE_HOOKS
#define DELAYIMP_INSECURE_WRITABLE_HOOKS
#endif

#include <delayimp.h>

FARPROC WINAPI MyDliNotifyHook(unsigned dliNotify, PDelayLoadInfo pdli)
{
  if (dliNotify == dliNotePreLoadLibrary)
  {
    return (FARPROC)LoadLibraryA(pdli->szDll);
  }

  return nullptr;
}

extern "C" PfnDliHook __pfnDliNotifyHook2 = MyDliNotifyHook;

// BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
// {
//   return TRUE;
// }