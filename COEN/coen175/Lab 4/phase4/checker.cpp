/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <map>
# include <set>
# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"


using namespace std;

static set<string> functions;
static map<string,Scope *> fields;
static Scope *outermost, *toplevel;
static const Type error;
static Scalar integer("int");

static string undeclared = "'%s' undeclared";
static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string incomplete = "'%s' has incomplete type";
static string nonpointer = "pointer type required for '%s'";

static string E1 = "invalid return type";
static string E2 = "invalid type for test expression";
static string E3 = "lvalue required in expression";
static string E4 = "invalid operands to binary %s";
static string E5 = "invalid operand to unary %s";
static string E6 = "invalid operand in cast expression";
static string E7 = "called object is not a function";
static string E8 = "invalid arguments to called function";
static string E9 = "using pointer to incomplete type";

# define isStructure(t) (t.isStruct() && t.indirection() == 0)


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;

    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	openStruct
 *
 * Description:	Open a scope for a structure with the specified name.  If a
 *		structure with the same name is already defined, delete it.
 */

void openStruct(const string &name)
{
    if (fields.count(name) > 0) {
	delete fields[name];
	fields.erase(name);
	report(redefined, name);
    }

    openScope();
}


/*
 * Function:	closeStruct
 *
 * Description:	Close the scope for the structure with the specified name.
 */

void closeStruct(const string &name)
{
    fields[name] = closeScope();
}


/*
 * Function:	declareSymbol
 *
 * Description:	Declare a symbol with the specified NAME and TYPE.  Any
 *		erroneous redeclaration is discarded.  If a declaration has
 *		multiple errors, only the first error is reported.  To
 *		report multiple errors, remove the "return" statements and,
 *		if desired, the final "else".
 */

void declareSymbol(const string &name, const Type &type, bool isParameter)
{
    Symbol *symbol = toplevel->find(name);

    if (symbol == nullptr)
	toplevel->insert(new Symbol(name, type));
    else if (toplevel != outermost) {
	report(redeclared, name);
	return;
    } else if (type != symbol->type()) {
	report(conflicting, name);
    	return;
    }

    if (isStructure(type)) {
	if (isParameter || type.isCallback() || type.isFunction())
	    report(nonpointer, name);
	else if (fields.count(type.specifier()) == 0)
	    report(incomplete, name);
    }
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  This
 *		definition always replaces any previous definition or
 *		declaration.  In the case of multiple errors, only the
 *		first error is reported.  To report multiple errors, remove
 *		the "else"s.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    Symbol *symbol = outermost->find(name);

    if (functions.count(name) > 0)
	report(redefined, name);
    else if (symbol != nullptr && type != symbol->type())
	report(conflicting, name);
    else if (isStructure(type))
	report(nonpointer, name);

    outermost->remove(name);
    delete symbol;

    symbol = new Symbol(name, type);
    outermost->insert(symbol);

    functions.insert(name);
    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}

//Finished
Type checkIndex(const Type &left, const Type &right) {
    if (left.isError() || right.isError())
        return error;

    Type newLeft = left.promote(), newRight = right.promote();

    if (newLeft.indirection() == 1 && fields.count(newLeft.specifier()) == 0 && newLeft.isStruct() && newLeft.isScalar()) {
        report(E9);
        return error;
    }
    else if (newRight.isInteger() && newLeft.isPointer())
        return Scalar(newLeft.specifier(), newLeft.indirection() - 1);
    report(E4, "[]");
    return error;
}


//Finished
Type checkCall(const Type &left, Parameters *p) {
    if (left.isError())
        return error;
    else if (left.isFunction() == false && left.isCallback() == false) {
        report(E7);
        return error;
    }

    for (auto &i :*p) {
        if (i.isValue() == false) {
            report(E8);
            return error;
        }
    }

    if (left.parameters() == nullptr)
        return Scalar(left.specifier(), left.indirection());
    else {
        if (left.parameters()->size() != p->size()) {
            report(E8);
            return error;
        }

        for (unsigned i = 0; i < p->size(); i++) {
            if (left.parameters()->at(i).isCompatibleWith(p->at(i)) == false) {
                report(E8);
                return error;
            }
        }
    }
    
    return Scalar(left.specifier(), left.indirection());
}

//Finished
Type checkDirectStructRef(const Type &left, const string &id) {
    if (left.isError())
        return error;
    else if (isStructure(left) && left.isScalar() && fields.count(left.specifier()) != 0) {
        Symbol *symbol = fields[left.specifier()]->find(id);
        if (symbol != nullptr)
            return symbol->type();
    }
    report(E4, ".");
    return error;
}

//Finished
Type checkIndirectStructRef(const Type &left, const string &id) {
    if (left.isError())
        return error;
    Type promotedLeft = left.promote();
    if (fields.count(promotedLeft.specifier()) == 0) {
        report(E9);
        return error;
    }
    Symbol *symbol = fields[promotedLeft.specifier()]->find(id);
    if ((promotedLeft.indirection() != 1 || promotedLeft.isStruct() == false || promotedLeft.isScalar() == false) || (symbol == nullptr)) {
        report(E4, "->");
        return error;
    }
    return symbol->type();
}

//Finished
Type checkLogicalOr(const Type &left, const Type &right) {
    if (left.isError() || right.isError())
        return error;
    if (left.isValue() && right.isValue())
        return integer;
    report(E4, "||");
    return error;
}

//Finished
Type checkLogicalAnd(const Type &left, const Type &right) {
    if (left.isError() || right.isError())
        return error;
    if (left.isValue() && right.isValue())
        return integer;
    report(E4, "&&");
    return error;
}

//Finished
Type checkEqual(const Type &left, const Type &right) {
    if (left.isError() || right.isError())
        return error;

    else {
        if (left.isCompatibleWith(right))
            return integer;
    }
    report(E4, "==");
    return error;
}

//Finished
Type checkNotEqual(const Type &left, const Type &right) {
    if (left.isError() || right.isError())
        return error;

    else {
        if (left.isCompatibleWith(right))
            return integer;
    }
    report(E4, "!=");
    return error;
}

//Finished
Type checkMul(const Type &left, const Type &right, const string &op) {
    if (left.isError() || right.isError())
        return error;
    else {
        Type promotedLeft = left.promote(), promotedRight = right.promote();
        if (promotedLeft == integer && promotedRight == integer)
            return integer;
    }
    report(E4, op);
    return error;
}

//Finished
Type checkNot(const Type &right) {
    if (right.isError())
        return error;
    if (right.isValue())
        return integer;
    report(E5, "!");
    return error;
}

//Finished
Type checkNeg(const Type &right) {
    if (right.isError())
        return error;
    Type promotedRight = right.promote();
    if (promotedRight == integer)
        return integer;
    report(E5, "-");
    return error;
}

//Finished
Type checkDeref(const Type &right) {
    if (right.isError())
        return error;
    Type promotedRight = right.promote();
    if (right.isScalar() && right.indirection() == 1 && right.isStruct() && fields.count(right.specifier()) == 0) {
        report(E9);
        return error;
    }
    else {
        if (promotedRight.isPointer())
            return Scalar(promotedRight.specifier(), promotedRight.indirection() - 1);
    }
    report(E5, "*");
    return error;
}

//Finished
Type checkAddr(const Type &right, const bool &lvalue) {
    if (right.isError())
        return error;
    else if (lvalue == false) {
        report(E3);
        return error;
    }
    else {
        if (right.isCallback()) {
            report(E5, "&");
            return error;
        }
    }

    return Scalar(right.specifier(), right.indirection() + 1);
}

//Finished
Type checkSizeOf(const Type &right) {
    if (right.isError())
        return error;
    if ((right.isFunction()) || (fields.count(right.specifier()) == 0 && isStructure(right))) {
        report(E5, "sizeof");
        return error;
    }

    return integer;
}

//Finished
Type checkCast(const Type &right, const Type &type) {
    if (right.isError())
        return error;
    Type promotedRight = right.promote();
    if ((type == integer && promotedRight == integer) || (type.isPointer() && promotedRight.isPointer()))
        return Scalar(type.specifier(), type.indirection());
    report(E6);
    return error;
}

//Finished
Type checkAdd(const Type &left, const Type &right, const string &op) {
    if (left.isError() || right.isError())
        return error;

    Type promotedRight = right.promote(), promotedLeft = left.promote();

    if ((promotedRight.indirection() == 1 && promotedRight.isScalar() && fields.count(promotedRight.specifier()) == 0 && promotedRight.isStruct()) ||
        (promotedLeft.isStruct() && promotedLeft.isScalar() && promotedLeft.indirection() == 1 && fields.count(promotedLeft.specifier()) == 0)) {
        report(E9);
        return error;
    }

    if ((promotedRight == integer && promotedLeft == integer) || (op == "-" && promotedLeft.isPointer() && promotedLeft == promotedRight))
        return integer;
    if (promotedLeft.isPointer() && promotedRight == integer)
        return promotedLeft;
    if (op == "+" && promotedLeft == integer && promotedRight.isPointer())
        return promotedRight;
    report(E4, op);
    return error;
}

//Finished
Type checkEqOrRelation(const Type &left, const Type &right, const string &op) {
    if (left.isError() || right.isError())
        return error;
    else {
        if (left.isCompatibleWith(right))
            return integer;
    }
    report(E4, op);
    return error;
}

//Finished
Type checkAssignment(const Type &left, const Type &right, const bool &lvalue) {
    if (left.isError() || right.isError())
        return error;
    else {
        if (lvalue == false) {
            report(E3);
            return error;
        }
        if (left.isCompatibleWith(right))
            return left;
    }

    report(E4, "=");
    return error;
}

//Finished
void checkReturn (const Type &type, const Type &returnType) {
    if (returnType.isError() || type.isError())
        return;
    else {
        Scalar newType(returnType.specifier(), returnType.indirection());
        if (type.isCompatibleWith(newType) == false)
            report(E1);
    }
}

//Finished
void checkIf(const Type &type) {
    if (type.isError())
        return;
    if (type.isValue() == false)
        report(E2);
}