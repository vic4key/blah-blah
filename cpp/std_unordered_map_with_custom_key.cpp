// In C++, a custom key for std::unordered_map required:
//  - Overload the comparison operator (==)
//  - A hash function (can support via std::hash)

#include <iostream>
#include <unordered_map>

struct MyKey
{
    MyKey(int i) : value(i) {}

    // Overload the comparison operator
    bool operator==(const MyKey& right) const
    { 
        return value == right.value;
    } 

    // Define a hash function for custom key
    struct Hash
    {
        std::size_t operator()(const MyKey& key) const
        {
            return std::hash<int>()(key.value);
        }
    };

    int value;
};

int main() {
    std::unordered_map<MyKey, std::string, MyKey::Hash> myMap;

    myMap[MyKey(1)] = "one";
    myMap[MyKey(2)] = "two";
    myMap[MyKey(3)] = "three";

    for (const auto& kv : myMap)
    {
        std::cout << kv.first.value << " => " << kv.second << std::endl;
    }

    return 0;
}