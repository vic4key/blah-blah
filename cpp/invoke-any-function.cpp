#include <any>
#include <functional>

struct Invokable
{
  Invokable() {}

  template<typename Function>
  Invokable(Function&& function) : Invokable(std::function(std::forward<Function>(function))) {}

  template<typename Return, typename ... Args>
  Invokable(std::function<Return(Args...)> function) : m_function(function)
  {
    m_function = function;
  }

  template<typename Return, typename ... Args>
  Return invoke(Args ... args)
  {
    auto function = std::any_cast<std::function<Return(Args...)>>(m_function);

    if (std::is_void<Return>::value)
    {
      std::invoke(function, std::forward<Args>(args)...);
    }
    else
    {
      return std::invoke(function, std::forward<Args>(args)...);
    }
  }

  std::any m_function;
};



#include <map>
#include <string>

std::map<std::string, Invokable> map_func;

template<typename Function>
void __store_func(const std::string& name, Function&& function)
{
  map_func[name] = std::function(std::forward<Function>(function));
}

template<typename Return = void, typename ... Args>
Return __invoke_func(const std::string& name, Args ... args)
{
  if (std::is_void<Return>::value)
  {
    map_func[name].invoke<Return>(std::forward<Args>(args)...);
  }
  else
  {
    return map_func[name].invoke<Return>(std::forward<Args>(args)...);
  }
}



void f_void(int x, int y)
{
  vu::print_A(__func__, x, y);
}

float f_type(int x, int y, int z)
{
  vu::print_A(__func__, x, y, z);
  return (x + y + z) * 1.F;
}

int main()
{
  using namespace std::literals::string_literals;
    
  __store_func("f_void"s, f_void);
  __store_func("f_type"s, f_type);

    int _1 = 1, _2 = 2, _3 = 3;
  __invoke_func<void>("f_void"s, _1, _2); // or `__invoke_func("f_void"s, _1, _2)`
  __invoke_func<float>("f_type"s, _1, 2, _3);

  return 0;
}
