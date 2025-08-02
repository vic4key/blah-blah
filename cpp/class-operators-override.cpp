#include <iostream>
#include <utility>

using namespace std;

class Foo
{
public:
    Foo()
    {
        cout << "Foo() ctor" << endl;
        m_data = 0;
    }

    Foo(const int data)
    {
        cout << "Foo(int data)  ctor" << endl;
        m_data = data;
    }

    Foo(const Foo& foo)
    {
        cout << "Foo(Foo& foo)  ctor - copying" << endl;
        m_data = foo.m_data;
    }

    Foo(Foo&& foo)
    {
        cout << "Foo(Foo&& foo) ctor - moving" << endl;
        m_data = foo.m_data;
        foo.m_data = 0;
    }

    virtual ~Foo()
    {
        // cout << "~Foo() dtor" << endl;
    }
 
    Foo& operator=(const Foo& rhs)
    {
        cout << "Foo& operator=(const Foo& rhs) - assignment" << endl;
        m_data = rhs.m_data;
        return *this;
    }

    friend istream& operator>>(istream& is, Foo& foo)
    {
        cout << "Foo& operator>> : "; is >> foo.m_data;
        return is;
    }

    friend ostream& operator<<(ostream& os, const Foo& foo)
    {
        os << "Foo& operator<< : " << foo.m_data;
        return os;
    }

private:
    int m_data;
};

int main()
{
    Foo foo(1);                     // Foo(int data)  ctor
    Foo foo_21(foo);                // Foo(Foo& foo)  ctor - copying
    Foo foo_22(std::move(foo));     // Foo(Foo&& foo) ctor - moving
    Foo foo_31 = foo;               // Foo(Foo& foo)  ctor - copying
    Foo foo_32 = std::move(foo);    // Foo(Foo&& foo) ctor - moving
    foo_31 = foo_32;                // Foo& operator=(const Foo& rhs) - assignment
    cin  >> foo;                    // istream& operator>>(istream& is, Foo& foo)
    cout << foo << endl;            // ostream& operator<<(ostream& os, const Foo& foo)
    return 0;
}
