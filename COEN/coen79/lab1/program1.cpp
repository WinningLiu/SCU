#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main()
{
    string text;
    int alphanum = 0, nonalphanum = 0;
    cout << "Please enter some text: ";
    getline(cin, text);
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] != ' ')
        {   
            if (isalnum(text[i]) == true)
                alphanum++;
            else
                nonalphanum++; 
        }
    }
    cout << "There are " << alphanum << " alphanumeric characters and " << nonalphanum << " nonalphanumeric characters in " << text << ".\n";
    return 0;
}