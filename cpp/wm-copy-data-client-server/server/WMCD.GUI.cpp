#include "WMCD.Interface.h"
#include "WMCD.h"

/**
 * Subclassing Window Procedure
 */

#define GWL_WNDPROC -4

static std::unordered_map<HWND, WNDPROC> HWNDWNDPROC;
static std::pair<LPVOID, SIZE_T> pVMBlock(nullptr, 0);

bool WMCDHandler(const WMCD::API api, const WMCD::Bytes& input, WMCD::Bytes& output);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COPYDATA:
    {
      auto pData = reinterpret_cast<PCOPYDATASTRUCT>(lParam);
      if (pData != nullptr)
      {
        // free previous memory block

        if (pVMBlock.first != nullptr && pVMBlock.second > 0)
        {
          MEMORY_BASIC_INFORMATION mbi = {0};
          VirtualQuery(pVMBlock.first, &mbi, sizeof(mbi));
          if (mbi.State == MEM_COMMIT)
          {
            VirtualFree(pVMBlock.first, pVMBlock.second, MEM_RELEASE);
          }
        }

        // copy input from window message buffer

        WMCD::Bytes output;
        WMCD::Bytes input(pData->cbData);
        CopyMemory(input.data(), pData->lpData, pData->cbData);

        // handle the calling api

        if (WMCDHandler(WMCD::API(pData->dwData), input, output))
        {
          if (!output.empty()) // api succeed with output
          {
            pVMBlock.first  = VirtualAlloc(NULL, output.size(), MEM_COMMIT, PAGE_READWRITE);
            pVMBlock.second = output.size();
            ZeroMemory(pVMBlock.first, pVMBlock.second);
            CopyMemory(pVMBlock.first, output.data(), output.size());
          }
          else // api succeed with no output
          {
            pVMBlock.first  = nullptr;
            pVMBlock.second = 0;
          }
        }
        else // api failed
        {
          pVMBlock.first  = nullptr;
          pVMBlock.second = -1;
        }

        PostThreadMessage(wParam, pData->dwData, WPARAM(pVMBlock.first), LPARAM(pVMBlock.second));
      }
    }
    break;

  default:
    break;
  }

  return CallWindowProc(HWNDWNDPROC[hwnd], hwnd, uMsg, wParam, lParam);
}

bool WMCDHandler(const WMCD::API api, const WMCD::Bytes& input, WMCD::Bytes& output)
{
  bool result = true;

  switch (api)
  {
  case WMCD::API::EXAMPLE:
    {
      MessageBoxP(std::wstring(input.cbegin(), input.cend()).data(), MBB_OK, MBI_INFO);
      std::string s = "Hello from Server";
      output.assign(s.cbegin(), s.cend());
    }
    break;

  default:
    break;
  }

  return result;
}

/**
 * WMCDGUI
 */

void WMCDGUI::Setup()
{
  auto hwnd = AfxGetMainWnd()->GetSafeHwnd();
  assert(IsWindow(hwnd));

  HWNDWNDPROC[hwnd] = (WNDPROC)GetWindowLongPtr(hwnd, GWL_WNDPROC);
  SetWindowLongPtr(hwnd, GWL_WNDPROC, LONG_PTR(WndProc));
}
