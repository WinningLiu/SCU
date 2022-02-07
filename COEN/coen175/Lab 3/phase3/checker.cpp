#include <iostream>
#include <string>
#include "checker.h"
#include "Scope.h"
#include "lexer.h"
#include <set>

using namespace std;

static Scope *global = nullptr;
static Scope *current = nullptr;

static set<string> structs;

static string E1 = "redefinition of '%s'";
static string E2 = "conflicting types for '%s'";
static string E3 = "redeclaration of '%s'";
static string E4 = "'%s' undeclared";
static string E5 = "pointer type required for '%s'";
static string E6 = "'%s' has incomplete type";

void openScope() {
    cout << "opening scope" << endl;
    current = new Scope(current);
    // if global is null, set global to current scope
    if (global == nullptr)
        global = current;
}

void closeScope() {
    cout << "closing scope" << endl;
    if (current != global) {
        Scope *temp = current;
        current = current->enclosing();
        delete(temp);
    }
    else if (current == global) {
        delete(current);
        current = nullptr;
        global = nullptr;
    }
    else if (global == nullptr)
        return;
}

void openStruct(const string &name) {
    if (structs.count(name) > 0)
        report(E1, name);
    openScope();
}

void closeStruct(const string &name) {
    closeScope();
    structs.insert(name);
}

//only in global scope
void declareFunction(const string &name, const Type &type) {
    cout << name << ": " << type << endl;
    Symbol *symbol = global->find(name);

    if (symbol == nullptr) {
        checkIfStruct(name, type);
        symbol = new Symbol(name, type);
        global->insert(symbol);
    }
    else if (type != symbol->type())
        report(E2, name);
    else 
        checkIfStruct(name, type);
}

//check for e5 and e1
void defineFunction(const string &name, const Type &type) {
    cout << name << ": " << type << endl;
    Symbol *symbol = global->find(name);
    if (symbol == nullptr) {
        checkIfStruct(name, type);
        symbol = new Symbol(name, type);
        global->insert(symbol);
    }
    else {
        if (type != symbol->type())
            report(E2, name);
        else if (symbol->type().parameters() != nullptr)
            report(E1, name);
        else
            checkIfStruct(name, type);
            
        global->remove(name);
        symbol = new Symbol(name, type);
        global->insert(symbol);
    }
    
}

void declareVariable(const string &name, const Type &type) {
    cout << name << ": " << type << endl;
    Symbol *symbol = current->find(name);

    if (symbol == nullptr) {
        if (type.isCallback())
            checkIfStruct(name, type);
        else if (type.isStruct() && structs.count(type.specifier()) == 0 && type.indirection() == 0)
            report(E6, name);
        symbol = new Symbol(name, type);
        current->insert(symbol);
    }
    else if (current->enclosing() != nullptr)
        report(E3, name);
    else if (type != symbol->type())
        report(E2, name);
}

void declareParameter(const string &name, const Type &type) {
    cout << name << ": " << type << endl;
    Symbol *symbol = current->find(name);
    if (symbol == nullptr) {
        checkIfStruct(name, type);
        symbol = new Symbol(name, type);
        current->insert(symbol);
    }
    else
        report(E3, name);
}

void checkIfStruct(const string &name, const Type &type) {
    if (type.indirection() == 0 && type.isStruct())
        report(E5, name);
}

void checkID(const string &name) {
    cout << "check " << name << endl;
    Symbol *symbol = current->lookup(name);
    if (symbol == nullptr)
        report(E4, name);
}