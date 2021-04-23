/**
 * The shared header for both client & server
 */

#pragma once

#include <vector>

#ifdef WMCDAPI_EXPORTS
#define WMCDAPI __declspec(dllexport)
#else
#define WMCDAPI __declspec(dllimport)
#endif

namespace WMCD
{

#ifndef WM_APP
#if(WINVER >= 0x0400)
#define WM_APP 0x8000
#endif /* WINVER >= 0x0400 */
#endif

enum API
{
  UNUSED = WM_APP,
  EXAMPLE,
  COUT,
};

typedef std::vector<char> Bytes;

#ifndef WMCD_GUI

void WMCDAPI Example(); // Eg. Call this to call the example API in any client app. WMCD::Example();

#endif // WMCD_GUI

}; // WMCD