//This assumes that all of the files are correct
// g++ -std=c++11 lexer.cpp

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <set>
#include <stdio.h>
#include <ctype.h>
using namespace std;

int main() {

    set<string> keywords = {"auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", 
    "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", 
    "union", "unsigned", "void", "volatile", "while"};

    set<char> single_operators = {'=', '|', '<', '>', '+', '-',
    '*', '/', '%', '&', '!', '.', '(', ')', '[', ']', '{', '}', ';', ':', ','};
    set<string> double_operators = {"||", "&&", "<=", ">=", "!=", "==", "->", "++", "--"};

    string temp;
    char c;

    while (!cin.eof()) {
        c = cin.get();
        temp = "";

        // whitespace, ignore
        if (isspace(c))
            continue;

        // numbers
        else if (isdigit(c)) {
            temp += c;
            while (isdigit(cin.peek()))
                temp += cin.get();
            cout << "number:" << temp << endl;
        }
        // strings
        else if (c == '"') {
            temp += c;
            while (cin.peek() != '"') {
                if (cin.peek() == '\\')
                    temp += cin.get();
                temp += cin.get();
            }
                
            temp += cin.get();
            cout << "string:" << temp << endl;
        }

        // character
        else if (c == '\'') {
            temp += c;
            while (cin.peek() != '\'') {
                if (cin.peek() == '\\')
                    temp += cin.get();
                temp += cin.get();
            }

            temp += cin.get();
            cout << "character:" << temp << endl;
        }

        // identifier and keywords
        else if (c == '_' || isalpha(c)) {
            temp += c;
            while (cin.peek() == '_' || isalpha(cin.peek()) || isdigit(cin.peek()))
                temp += cin.get();
            if (keywords.find(temp) != keywords.end())
                cout << "keyword:" << temp << endl;
            else
                cout << "identifier:" << temp << endl;
        }

        // comments
        else if (c == '/' and cin.peek() == '*') {
            temp += c;
            temp += cin.get();
            while (true){
                c = cin.get();
                temp += c;
                if (c == '*' && cin.peek() == '/') {
                    cin.get();
                    break;
                }
            }
        }

        // operators
        else if (single_operators.find(c) != single_operators.end()){
            temp += c;
            c = cin.peek();
            if (double_operators.find(temp + c) != double_operators.end())
                temp += cin.get();
            cout << "operator:" << temp << endl;
        }

        // anything else, ignore
        else {
            continue;
        }
        
    }

    return 0;
}