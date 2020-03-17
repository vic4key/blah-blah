#include <iostream>
#include <sstream>

class stream_t
{
public:
  stream_t()
  {
  }

  virtual ~stream_t()
  {
  }

  std::stringstream& stream()
  {
    return m_stream;
  }

  std::string str()
  {
    return m_stream.str();
  }

  template<typename T>
  friend stream_t& operator<<(stream_t& stream, T v)
  {
    stream.stream() << v;
    return stream;
  }

  friend stream_t& operator<<(stream_t& stream, std::ostream& (*v)(std::ostream&))
  {
    if (v == std::endl)
    {
      stream.stream() << '\n';
    }

    return stream;
  }

  friend stream_t& operator<<(stream_t& stream, float v)
  {
    char s[100] = { 0 };
    sprintf_s(s, "%0.1f", v);
    stream.stream() << s;
    return stream;
  }

  friend stream_t& operator<<(stream_t& stream, double v)
  {
    stream << float(v);
    return stream;
  }

private:
  std::stringstream m_stream;
};

int main()
{
  stream_t ss;

  int i = 123;
  float f = 123.456F;
  double d = 456.789;

  ss << "i: " << i << std::endl;
  ss << "f: " << f << std::endl;
  ss << "d: " << d << std::endl;

  std::cout << ss.str();

  return 0;
}