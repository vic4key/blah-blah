#include <iostream>
using namespace std;

bool search_value_in_array_from_center(int a[4][4], int n, int m, int v, int& x, int& y)
{
    y = -1;
    x = -1;

    if (n == 0 || m == 0)
    {
        return false;
    }

    int i = 0, j = m - 1;

    while (i < n && j >= 0)
    {
        for (int k = j; k >= 0; k--)
        {
            if (a[i][k] == v)
            {
                y = i;
                x = k;
                return true;
            }
        }

        i++;

        for (int k = i; k < n; k++)
        {
            if (a[k][j] == v)
            {
                y = k;
                x = j;
                return true;
            }
        }

        j--;
    }

    return false;
}

int main()
{
    int a[4][4] =
    {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 },
        { 13, 14, 15, 16 },
    };

    int x = -1, y = -1;
    bool found = search_value_in_array_from_center(a, 4, 4, 11, x, y);
    cout << "Found? " << (found ? "Yes" : "No") << " at " << x << ", " << y << endl;

    return 0;
}