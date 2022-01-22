// power-suspended-resumed-monitor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vu>

#include <windows.h>
#include <powrprof.h>

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registersuspendresumenotification

ULONG CALLBACK fn_notify_callback_routine(PVOID Context, ULONG Type, PVOID Setting)
{
  switch (Type)
  {
  case PBT_APMRESUMEAUTOMATIC:
    {
      vu::msg_debug_A("Operation resuming automatically after event.");
    }
    break;

  case PBT_APMRESUMESUSPEND:
    {
      vu::msg_debug_A("Operation resuming after suspension.");
    }
    break;

  case PBT_APMSUSPEND:
    {
      vu::msg_debug_A("System is suspending operation.");
    }
    break;

  default:
    break;
  }

  return 0;
}

int main()
{
  DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS dnsp = { 0 };
  dnsp.Callback = fn_notify_callback_routine;
  RegisterSuspendResumeNotification(&dnsp, DEVICE_NOTIFY_CALLBACK);

  MessageBoxA(nullptr, "keep running ...", "", MB_OK);

  return 0;
}
