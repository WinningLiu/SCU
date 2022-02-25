#include "Tree.h"
#include "generator.h"
#include <iostream>

using namespace std;

static ostream &operator<<(ostream &ostr, Expression *expr) {
    expr->operand(ostr);
    return ostr;
}

void Procedure::generate() {
    Symbols symbols = _body->declarations()->symbols();
    int n = _id->type().parameters()->size();
    unsigned count = 0;
    int total_offset = 0;

    //parameters
    for (auto i : symbols) {
        if (n > 0 && count == 0) {
            total_offset += 8;
            i->offset += total_offset;
            total_offset += i->type().size();
            count = 1;
            n--;
        }
        else if (n > 0 && count != 0) {
            i->offset = total_offset;
            total_offset += i->type().size();
            n--;
        }
        else {
            break;
        }
    }

    total_offset = 0;
    n = _id->type().parameters()->size();

    for (auto i : symbols) {
        if (n > 0)
            n--;
        else if (count == 0) {
            total_offset -= i->type().size();
            i->offset += total_offset;
            count = 1;
        }
        else {
            total_offset -= i->type().size();
            i->offset = total_offset;
        }
    }

    cout << _id->name() << ":" << endl;
    cout << "pushl\t%ebp" << endl;
    cout << "movl\t%esp, %ebp" << endl;
    cout << "subl\t$" << total_offset*-1 << ", %esp" << endl;
    _body->generate();
    cout << "movl\t%ebp, %esp" << endl;
    cout << "popl\t%ebp" << endl;
    cout << "ret" << endl;
}

void Simple::generate() {
    _expr->generate();
}

void Block::generate() {
    for (auto stmt : _stmts)
        stmt->generate();
}

void Assignment::generate() {
    _right->generate();
    _left->generate();
    cout << "movl\t" << _right << ", " << _left << endl;
}

void Call::generate() {
    for (int i = _args.size() - 1; i >= 0; i--) {
        _args[i]->generate();
        cout << "pushl\t" << _args[i] << endl;
    }

    cout << "call\t" << _expr << endl;

    cout << "addl\t" << "$" << _args.size()*4 << ", %esp" << endl;
}

void generateGlobals(Scope *global) {
    Symbols symbols = global->symbols();
    for (auto symbol : symbols) {
        if (symbol->type().isFunction() == false)
            cout << ".comm " << symbol->name() << ", " << symbol->type().size() << endl;
        else {
            cout << ".globl\t" << symbol->name() << endl;
        }
    }
}

void Identifier::operand(ostream &ostr) {
    int offset = _symbol->offset;
    if (offset == 0)
        cout << _symbol->name();
    else
        cout << offset << "(%ebp)";
}

void Number::operand(ostream &ostr) {
    ostr << "$" << _value;
}

void Expression::operand(ostream &ostr) {

}