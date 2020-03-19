#include <iostream>
#include <sstream>

class stream_t
{
public:
  typedef std::ostream& (*fn)(std::ostream&);

  stream_t()
  {
  }

  stream_t(stream_t& right)
  {
    *this = right;
  }

  virtual ~stream_t()
  {
  }

  stream_t& operator=(stream_t& right)
  {
    m_stream.str(right.str());
    return *this;
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

  friend stream_t& operator<<(stream_t& stream, std::ostream& (*pfn)(std::ostream&))
  {
    if (pfn == fn(std::endl))
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