#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        cout << "Please enter a text file.";
        return 1;
    }
    ifstream in_file;
    in_file.open("text1.txt");

    string word = "";
    string tempstring;
    while (in_file && in_file.peek() != EOF)
    {
        word = "";
        in_file >> tempstring;
        for (int i = 0; i < tempstring.length(); i++)
        {
            if (isalpha(tempstring[i]))
                word += toupper(tempstring[i]);
        } 
        if (word.length() >= 10)
            cout << word << endl;
    } 
    return 0;
}