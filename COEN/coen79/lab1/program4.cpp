#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main()
{
    string lmao;
    string response;
    int counter = 1;
    cout << "Think of a number between 1 and 20. Press enter when you are ready.\n";
    cin.ignore();
    while (true)
    {
        int number = rand() % 20 + 1;
        cout << "Is the number " << number << " ? Y or N\n";
        cin >> response;
        if (response == "yes" || response == "Y" || response == "y")
            break;
        counter++;
    }
        cout << "I found the number in " << counter << " steps.\n";
}