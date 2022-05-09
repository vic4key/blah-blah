#include <iostream>
using namespace std;

#pragma warning(push)
#pragma warning(disable: 4267)
#include "WinHttpWrapper\WinHttpWrapper.h"
using namespace WinHttpWrapper;
#pragma warning(pop)

#include "winhttp-wrapper\net.h"

int main()
{
  std::wstring header;
  header.append(L"Accept: */*\r\n");
  header.append(L"Accept-Encoding: *\r\n");
  header.append(L"Accept-Language: *\r\n");
  header.append(L"Connection: keep-alive\r\n");
  header.append(L"\r\n");

  // WinHttpWrapper @ https://www.codeproject.com/Articles/5280036/Making-HTTP-REST-Request-in-Cplusplus-With-WinHTTP
  // https://github.com/shaovoon/winhttp_examples
  HttpRequest  req(L"api.github.com", 443, true);
  HttpResponse response;
  req.Get(L"/repos/vic4key/GL-Window/releases/latest", header, response);
  cout << response.statusCode << endl;
  cout << response.text << endl;
  response.Reset();

  // winhttp-wrapper @ https://github.com/vcappello/winhttp-wrapper.git
  auto data = net::fetch_request(net::url(L"https://api.github.com/repos/vic4key/GL-Window/releases/latest"));
  std::string s(data.begin(), data.end());
  cout << s << endl;

  return 0;
}