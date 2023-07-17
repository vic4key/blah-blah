// exe-data-seg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

__declspec(dllimport) int my_var;

int main()
{
  while (true)
  {
    int s = 0;
    cout << "\n1: set, 2: get\nselect: "; cin >> s;
    if (s == 1)
    {
      cout << "set value: "; cin >> my_var;
    }
    else
    {
      cout << "get value: " << my_var << endl;
    }
  }

  return 0;
}
