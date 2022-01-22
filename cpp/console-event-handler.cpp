// console-event-handler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <windows.h>

BOOL WINAPI fn_ctrl_handler(DWORD fdwCtrlType)
{
  switch (fdwCtrlType)
  {
  case CTRL_C_EVENT: // handle the CTRL-C signal.
    printf("Ctrl-C event\n");
    return TRUE;

  case CTRL_CLOSE_EVENT: // confirm that the user wants to exit.
    printf("Ctrl-Close event\n");
    return TRUE;

  case CTRL_BREAK_EVENT: // Pass other signals to the next handler.
    printf("Ctrl-Break event\n");
    return FALSE;

  case CTRL_LOGOFF_EVENT:
    printf("Ctrl-Logoff event\n");
    return FALSE;

  case CTRL_SHUTDOWN_EVENT:
    printf("Ctrl-Shutdown event\n");
    return FALSE;

  default:
    return FALSE;
  }
}

void fn_signal_handler(int signum)
{
  printf("signum = %d\n", signum);
  exit(signum); // (opt)
}

int main()
{
  signal(SIGINT, fn_signal_handler);
  // or
  SetConsoleCtrlHandler(fn_ctrl_handler, TRUE);

  MessageBoxA(nullptr, "keep running ...", "", MB_OK);

  return 0;
}
