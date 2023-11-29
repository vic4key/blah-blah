// In C++, a custom key for std::map required:
//  - Overload the less than operator (<)

#include <iostream>
#include <map>

struct MyKey
{
    MyKey(int i) : value(i) {}

    // Overload the less than operator
    bool operator<(const MyKey& right) const
    {
        return value < right.value;
    }

public:
    int value;
};

int main() {
    std::map<MyKey, std::string> myMap;

    myMap[MyKey(1)] = "one";
    myMap[MyKey(2)] = "two";
    myMap[MyKey(3)] = "three";

    for (const auto& kv : myMap)
    {
        std::cout << kv.first.value << " => " << kv.second << std::endl;
    }

    return 0;
}