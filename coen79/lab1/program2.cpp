#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main()
{
    string text;
    string space;
    cout << "Please type a string of 10 digits (0 to 9) with no space\n";
    cin >> text;
    for (int i = 0; i < 5; i++)
    {
        if (i == 1)
            space = " ";
        else if (i == 2)
            space = "  ";
        else if (i == 3)
            space = "   ";
        else if (i == 4)
            space = "    ";
        cout << space << text << "   ";
        for (int j = text.size() - 1; j >= 0; j--)
        {
            cout << text[j];
        }
            cout << "\n";
    }
    return 0;
}