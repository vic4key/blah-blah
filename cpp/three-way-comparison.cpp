// C++ 20 required

#include <iostream>
#include <vector>
#include <algorithm>

struct MyClass
{
    MyClass(int i) : value(i) {}

    std::strong_ordering operator<=>(const MyClass& right) const
    {
        if (value < right.value)
        {
            return std::strong_ordering::less;
        }
        else if (value > right.value)
        {
            return std::strong_ordering::greater;
        }
 
        return std::strong_ordering::equal;
    }

    int value;
};

int main()
{
    std::vector<MyClass> myVector = { MyClass(3), MyClass(1), MyClass(2) };

    std::sort(myVector.begin(), myVector.end());

    for (const auto& obj : myVector)
    {
        std::cout << obj.value << " ";
    }

    return 0;
}