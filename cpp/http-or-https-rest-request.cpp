// https://www.codeproject.com/Articles/5280036/Making-HTTP-REST-Request-in-Cplusplus-With-WinHTTP
// https://github.com/shaovoon/winhttp_examples

#include <iostream>
using namespace std;

#pragma warning(push)
#pragma warning(disable: 4267)
#include "WinHttpWrapper\WinHttpWrapper.h"
using namespace WinHttpWrapper;
#pragma warning(pop)

int main()
{
  std::wstring header;
  header.append(L"Accept: */*\r\n");
  header.append(L"Accept-Encoding: *\r\n");
  header.append(L"Accept-Language: *\r\n");
  header.append(L"Connection: keep-alive\r\n");
  header.append(L"\r\n");

  HttpRequest  req(L"api.github.com", 443, true);
  HttpResponse response;

  req.Get(L"/repos/vic4key/GL-Window/releases/latest", header, response);
  cout << response.statusCode << endl;
  cout << response.text << endl;
  response.Reset();

  return 0;
}