#include <functional>
#include <typeinfo>
#include <any>
#include <map>
#include <vu>

using namespace std;
using namespace vu;

class B
{
public:
  virtual void f0() { print(__FUNCSIG__); }
  virtual void f1(int a) { print(__FUNCSIG__); }
  virtual void f2(int a, char b) { print(__FUNCSIG__); }
  virtual void f3(int a, char b, float c) { print(__FUNCSIG__); }
};

class D : public B
{
public:
  virtual void f0() { print(__FUNCSIG__); }
  virtual void f1(int a) { print(__FUNCSIG__); }
  virtual void f2(int a, char b) { print(__FUNCSIG__); }
  virtual void f3(int a, char b, float c) { print(__FUNCSIG__); }
};

std::map<string, std::any> m;

template <typename ...Args>
void invoke(const std::string& method, Args... args)
{
  print(method, "=>", m[method].type().name(), "=>", args...);
  // (m[method])(p, args...); // TODO: Vic. Recheck. Have not yet a solution to make a call.
}

int main()
{
  m["f0"] = &B::f0;
  m["f1"] = &B::f1;
  m["f2"] = &B::f2;
  m["f3"] = &B::f3;

  B* p = new D;
  invoke("f0");
  invoke("f1", 1);
  invoke("f2", 1, '2');
  invoke("f3", 1, '2', 3.F);
  delete p;

  return 0;
}