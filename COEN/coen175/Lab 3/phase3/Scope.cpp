#include "Scope.h"

using namespace std;

Scope::Scope(Scope *enclosing)
    :_enclosing(enclosing)
{

}

// only in the current scope
Symbol *Scope::find(const string &name) const {
    for (auto const &i : _symbols) {
        if (i->name() == name)
            return i;
    }

    return nullptr;
}

// searches in scopes above current scope
Symbol *Scope::lookup(const string &name) const {
    //searches current scope
    Symbol *found = find(name);
    if (found != nullptr)
        return found;

    else if (_enclosing != nullptr)
        return _enclosing->lookup(name);
    return nullptr;
}

void Scope::insert(Symbol *symbol) {
    _symbols.push_back(symbol);
}

void Scope::remove(const string &name) {
    Symbol *symbol = find(name);
    if (symbol != nullptr) {
        for (unsigned i = 0; i < _symbols.size(); i++) {
            if (_symbols[i]->name() == name) {
                _symbols.erase(_symbols.begin() + i);
                break;
            }
        }
    }
    
}