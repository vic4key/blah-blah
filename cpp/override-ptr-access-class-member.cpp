#include <iostream>

class X
{
public:
  void f()
  {
    std::cout << __FUNCSIG__ << std::endl;
  }
};

class Y
{
public:
  X* operator->()
  {
    return &m_x;
  };

private:
  X m_x;
};

int main(int argc, char const *argv[])
{
  Y y;
  y->f(); // directly access method f of class X from class Y via operator ->
  
  return 0;
}