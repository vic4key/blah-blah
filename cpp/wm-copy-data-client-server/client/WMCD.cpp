#include "WMCD.h"
#include <Windows.h>

namespace WMCD
{

bool Call(const API api, const Bytes& input, Bytes& output)
{
  output.clear();

  if (!(api > API::UNUSED && api < API::COUT))
  {
    throw std::exception("API Unavailable");
  }

  auto hw = FindWindow(L"WMCDGUI", nullptr);
  if (hw == nullptr)
  {
    throw std::exception("Required Window GUI must be running");
  }

  DWORD pid = 0;
  GetWindowThreadProcessId(hw, &pid);
  if (pid == 0)
  {
    return false;
  }

  auto fnreturn = []()
  {
    std::pair<LPVOID, SIZE_T> result(nullptr, 0);

    MSG msg = {0};

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
      if (msg.message > API::UNUSED && msg.message < API::COUT)
      {
        result.first  = LPVOID(msg.wParam);
        result.second = SIZE_T(msg.lParam);
        break;
      }
      else if (msg.message == WM_QUIT)
      {
        result.first  = nullptr;
        result.second = 0;
        break;
      }
    }

    return result;
  };

  // send input to Window GUI and to call api

  COPYDATASTRUCT ds = {0};
  ds.dwData = api;
  ds.cbData = DWORD(input.size());
  ds.lpData = PVOID(input.data());

  SendMessage(hw, WM_COPYDATA, WPARAM(GetCurrentThreadId()), LPARAM(&ds));

  // get api result

  auto ret = fnreturn();
  if (ret.first == nullptr && ret.second == -1) // api failed
  {
    return false;
  }

  if (ret.first == nullptr && ret.second == 0) // api succeed with no output
  {
    return true;
  }

  // api succeed with output, copy api output from Window GUI then remotely free output memory

  auto hprocess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ, FALSE, pid);
  if (hprocess == INVALID_HANDLE_VALUE)
  {
    throw std::exception("Your program must be Run as Administrator");
  }

  output.resize(ret.second);

  SIZE_T bytes = 0;
  ReadProcessMemory(hprocess, ret.first, LPVOID(&output[0]), SIZE_T(ret.second), &bytes);

  VirtualFreeEx(hprocess, LPVOID(ret.first), 0, MEM_RELEASE);

  CloseHandle(hprocess);

  return true;
}

/* APIs */

void Example()
{
  try
  {
    std::string s = "Hello from Client";
    WMCD::Bytes output, input(s.cbegin(), s.cend());

    WMCD::Call(WMCD::API::EXAMPLE, input, output);

    std::string result(output.cbegin(), output.cend());
    ::MessageBoxA(::GetActiveWindow(), result.c_str(), "OK", MB_OK);
  }
  catch (const std::exception& e)
  {
    ::MessageBoxA(::GetActiveWindow(), e.what(), "Error", MB_OK | MB_ICONERROR);
  }
}

}