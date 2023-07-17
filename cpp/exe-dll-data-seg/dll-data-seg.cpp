// dll-data-seg.cpp : Defines the entry point for the DLL application.

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#pragma data_seg(".my_seg")
__declspec(dllexport) int my_var = 1609;
#pragma data_seg()
#pragma comment(linker, "/Section:.my_seg,RWS")

//BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//
//    return TRUE;
//}
