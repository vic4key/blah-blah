#include <functional>
#include <typeinfo>
#include <any>
#include <map>
#include <vu>

using namespace std::placeholders;
using namespace std;
using namespace vu;

class B
{
public:
  virtual void f() { print(__FUNCSIG__); }
  virtual void ff() { print(__FUNCSIG__); }
  virtual void fff(int a, char b, float c) { print(__FUNCSIG__); }
  virtual void ffff(int a, char b, float c, double d) { print(__FUNCSIG__); }
};

class D : public B
{
public:
  virtual void f() { print(__FUNCSIG__); }
  virtual void fff(int a, char b, float c) { print(__FUNCSIG__); }
  virtual void ffff(int a, char b, float c, double d) { print(__FUNCSIG__); }
};

B* p = nullptr;

auto fn_f = std::mem_fn(&B::f);
auto fn_ff = std::mem_fn(&B::ff);
auto fn_fff_args = std::mem_fn(&B::fff);
auto fn_ffff_args = std::mem_fn(&B::ffff);

std::map<string, std::_Mem_fn<void(B::*)()>*> m_none;
std::map<string, std::_Mem_fn<void(B::*)(int, char, float)>*> m_args;
std::map<string, std::any> m;

template <typename ...Args>
void invoke(const std::string& method, Args... args)
{
  print(m[method].type().name(), " => ", args...);

  // (m[method])(p, args...); // TODO: Vic. Recheck. Have not yet a solution to make a call.

   // typedef std::pair<bool, bool>(UI::Window::*pfn)(UINT idx, LPARAM lp);
   // auto method = std::any_cast<pfn>(method_ptr);
   // auto result = (*this.*method)(idx, lp);
}

int main()
{
  p = new D;

  // store class methods that SAME PROTOTYPE to a common map and callable with NO ARG

  m_none["f"] = &fn_f;
  m_none["ff"] = &fn_ff;

  (*m_none["f"])(p);
  invoke("f");

  // store class methods that SAME PROTOTYPE to a common map and callable with MULTIPLE ARGS

  m_args["fff"] = &fn_fff_args;
  invoke("fff", 1, '2', 3.f);

  // store class methods that DIFFER PROTOTYPE to a common map and callable with MULTIPLE ARGS

  m["f"] = &fn_f;
  m["ff"] = &fn_ff;
  m["fff"] = &fn_fff_args;
  m["fff"] = &fn_fff_args;
  m["ffff"] = &fn_ffff_args;

  invoke("ff");
  invoke("fff", 1, '2', 3.f);
  invoke("ffff", 1, '2', 3.f, 4.);

  delete p;
  return 0;
}