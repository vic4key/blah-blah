#define msg_debug_A(format, ...)\
{\
  char s[255] = { 0 };\
  memset(s, 0, sizeof(s));\
  sprintf_s(s, (char*)format, __VA_ARGS__);\
  OutputDebugStringA(s);\
}

#define msg_debug_W(format, ...)\
{\
  wchar_t s[255] = { 0 };\
  memset(s, 0, sizeof(s));\
  swprintf_s(s, (wchar_t*)format, __VA_ARGS__);\
  OutputDebugStringA(s);\
}

#ifdef _UNICODE
#define msg_debug msg_debug_W
#else
#define msg_debug msg_debug_A
#endif // _UNICODE