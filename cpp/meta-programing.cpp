#include <iostream>
#include <typeinfo>

using namespace std;

// factorial a number

template <int N>
struct factorial{
	static int const value = N * factorial<N-1>::value;
};

template <>
struct factorial<1>{
	static int const value = 1;
};

// square a number

template <int N>
struct square
{
	static const int value = N * N;
};

// un-const

template<typename T>
struct unconst
{
    typedef T type;
};

template<typename T>
struct unconst<const T>
{
	typedef T type;
};



int main(int argc, char const *argv[])
{
	cout << factorial<5>::value << endl;
	cout << square<5>::value << endl;
	cout << typeid(unconst<const int>::type).name() << endl;
	return 0;
}