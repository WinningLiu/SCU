/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 *		- scaling the operands and results of pointer arithmetic
 *		- explicit type promotions
 */

# include <map>
# include <set>
# include <cassert>
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
static const Scalar integer("int"), character("char");

static string undeclared = "'%s' undeclared";
static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string incomplete = "'%s' has incomplete type";
static string nonpointer = "pointer type required for '%s'";

static string invalid_return = "invalid return type";
static string invalid_test = "invalid type for test expression";
static string invalid_lvalue = "lvalue required in expression";
static string invalid_operands = "invalid operands to binary %s";
static string invalid_operand = "invalid operand to unary %s";
static string invalid_cast = "invalid operand in cast expression";
static string invalid_function = "called object is not a function";
static string invalid_arguments = "invalid arguments to called function";
static string incomplete_type = "using pointer to incomplete type";

# define isStructure(t) (t.isStruct() && t.indirection() == 0)


/*
 * Function:	promote
 *
 * Description:	Explicitly perform promotion on the given expression.  A
 *		function is promoted to a callback by inserting an address
 *		expression, an array is promoted to a pointer by inserting
 *		an address expression, and a character is promoted to
 *		integer by inserting a cast expression.
 */

static Type promote(Expression *&expr)
{
    if (expr->type().isFunction() || expr->type().isArray())
	expr = new Address(expr, expr->type().promote());
    else if (expr->type() == character)
	expr = new Cast(expr, expr->type().promote());

    return expr->type();
}


/*
 * Function:	scale
 *
 * Description:	Scale the result of pointer arithmetic.
 */

static Expression *scale(Expression *expr, unsigned size)
{
    unsigned value;


    if (size == 1)
	return expr;

    if (expr->isNumber(value)) {
	delete expr;
	return new Number(value * size);
    }

    return new Multiply(expr, new Number(size), integer);
}


/*
 * Function:	isIncompletePointer
 *
 * Description:	Check if the specified type is a pointer to an incomplete
 *		structure type.
 */

static bool isIncompletePointer(const Type &type)
{
    if (!type.isScalar() || type.indirection() != 1)
	return false;

    return type.isStruct() && fields.count(type.specifier()) == 0;
}


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
 * Function:	getFields
 *
 * Description:	Return the fields associated with the specified structure.
 */

Scope *getFields(const string &name)
{
    assert(fields.count(name) > 0);
    return fields[name];
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


/*
 * Function:	checkCall
 *
 * Description:	Check a function call expression: the type of the object
 *		being called must be a function or callback type, and the
 *		number and types of arguments and parameters must agree.
 */

Expression *checkCall(Expression *expr, Expressions &args)
{
    const Type &t = expr->type();
    Type result = error;
    Parameters *params;


    if (t != error) {
	if (t.isFunction() || t.isCallback()) {
	    params = t.parameters();
	    result = Scalar(t.specifier(), t.indirection());

	    for (unsigned i = 0; i < args.size(); i ++)
		if (!promote(args[i]).isValue()) {
		    report(invalid_arguments);
		    result = error;
		    break;
		}

	    if (params != nullptr && result != error) {
		if (params->size() != args.size())
		    report(invalid_arguments);

		else
		    for (unsigned i = 0; i < args.size(); i ++)
			if (!args[i]->type().isCompatibleWith((*params) [i])) {
			    report(invalid_arguments);
			    result = error;
			    break;
			}
	    }

	} else
	    report(invalid_function);
    }

    return new Call(expr, args, result);
}


/*
 * Function:	checkArray
 *
 * Description:	Check an array index expression: the left operand must have
 *		type "pointer to T" and the right operand must have type
 *		int, and the result has type T.  The pointer type must be
 *		complete.
 */

Expression *checkArray(Expression *left, Expression *right)
{
    Type t1 = left->type();
    Type t2 = right->type();
    Type result = error;


    if (t1 != error && t2 != error) {
	if (isIncompletePointer(t1))
	    report(incomplete_type);

	else if (t1.isPointer() && t2.isInteger()) {
	    t1 = promote(left);
	    right = scale(right, t1.deref().size());
	    result = t1.deref();

	} else
	    report(invalid_operands, "[]");
    }

    return new Dereference(new Add(left, right, t1), result);
}


/*
 * Function:	checkDirectField
 *
 * Description:	Check a direct field reference expression: expr . id.  The
 *		expression must have a structure type, the identifier must
 *		be a field of that structure, and the result has the type
 *		of the field.  If the identifier is not a member of the
 *		structure, we go ahead and declare it and give it the error
 *		type, so we only get the error once.
 */

Expression *checkDirectField(Expression *expr, const std::string &id)
{
    const Type &t = expr->type();
    Symbol *symbol = nullptr;


    if (t != error) {
	if (t.isStruct() && !t.indirection() && fields.count(t.specifier())) {
	    symbol = fields[t.specifier()]->find(id);

	    if (symbol == nullptr) {
		symbol = new Symbol(id, error);
		fields[t.specifier()]->insert(symbol);
		report(invalid_operands, ".");
	    }

	} else
	    report(invalid_operands, ".");
    }

    if (symbol == nullptr)
	symbol = new Symbol("-unknown-", error);

    return new Field(expr, symbol, symbol->type());
}


/*
 * Function:	checkIndirectField
 *
 * Description:	Check an indirect field reference expression: expr -> id.
 *		The expression must have type pointer(T), where T is a
 *		complete structure type, the identifier must be a field of
 *		that structure, and the result has the type of the field.
 *		Once again, if the identifier is not a member of the
 *		structure, we go ahead and declare it and give it the error
 *		type, so we only get the error once.
 */

Expression *checkIndirectField(Expression *expr, const std::string &id)
{
    Type t = promote(expr);
    Symbol *symbol = nullptr;


    if (t != error) {
	if (isIncompletePointer(t))
	    report(incomplete_type);

	else if (t.isStruct() && t.indirection() == 1) {
	    symbol = fields[t.specifier()]->find(id);
	    t = t.deref();

	    if (symbol == nullptr) {
		symbol = new Symbol(id, error);
		fields[t.specifier()]->insert(symbol);
		report(invalid_operands, "->");
	    }

	} else
	    report(invalid_operands, "->");
    }

    if (symbol == nullptr)
	symbol = new Symbol("-unknown-", error);

    return new Field(new Dereference(expr, t), symbol, symbol->type());
}


/*
 * Function:	checkNot
 *
 * Description:	Check a logical negation expression: the operand must have a
 *		value type, and the result has type int.
 */

Expression *checkNot(Expression *expr)
{
    const Type &t = promote(expr);
    Type result = error;


    if (t != error) {
	if (t.isValue())
	    result = integer;
	else
	    report(invalid_operand, "!");
    }

    return new Not(expr, result);
}


/*
 * Function:	checkNegate
 *
 * Description:	Check an arithmetic negation expression: the operand must
 *		have type int, and the result has type int.
 */

Expression *checkNegate(Expression *expr)
{
    const Type &t = promote(expr);
    Type result = error;


    if (t != error) {
	if (t.isInteger())
	    result = integer;
	else
	    report(invalid_operand, "-");
    }

    return new Negate(expr, result);
}


/*
 * Function:	checkDereference
 *
 * Description:	Check a dereference expression: * expr.  The operand must
 *		have type "pointer to T," where T is complete, and the
 *		result has type T.
 */

Expression *checkDereference(Expression *expr)
{
    const Type &t = promote(expr);
    Type result = error;


    if (t != error) {
	if (isIncompletePointer(t))
	    report(incomplete_type);
	else if (t.isPointer())
	    result = t.deref();
	else
	    report(invalid_operand, "*");
    }

    return new Dereference(expr, result);
}


/*
 * Function:	checkAddress
 *
 * Description:	Check an address expression: & expr.  The operand must be
 *		an lvalue, and if it has type T, then the result has type
 *		"pointer to T," where T cannot be a callback.
 */

Expression *checkAddress(Expression *expr)
{
    const Type &t = expr->type();
    Type result = error;


    if (t != error) {
	if (!expr->lvalue())
	    report(invalid_lvalue);
	else if (t.isCallback())
	    report(invalid_operand, "&");
	else
	    result = Scalar(t.specifier(), t.indirection() + 1);
    }

    return new Address(expr, result);
}


/*
 * Function:	checkSizeof
 *
 * Description:	Check a sizeof expression: sizeof type.  The operand cannot
 *		be a function type nor an incomplete structure type.
 */

Expression *checkSizeof(const Type &type)
{
    if (type.isFunction()) {
	report(invalid_operand, "sizeof");
	return new Number(0);
    }

    if (type.isStruct() && type.indirection() == 0)
	if (fields.count(type.specifier()) == 0) {
	    report(invalid_operand, "sizeof");
	    return new Number(0);
	}

    return new Number(type.size());
}


/*
 * Function:	checkCast
 *
 * Description:	Check a cast expression: (type) expr.  The result and
 *		operand must either both have numeric types or both have
 *		pointer types after any promotion.
 */

Expression *checkCast(const Type &type, Expression *expr)
{
    const Type &t = promote(expr);


    if (t == error || type == t)
	return expr;

    if (type.isInteger() && t.isInteger())
	return new Cast(expr, type);

    if (type.isPointer() && t.isPointer())
	return new Cast(expr, type);

    report(invalid_cast);
    return new Cast(expr, error);
}


/*
 * Function:	checkMultiplicative
 *
 * Description:	Check a multiplication expression: both operands must have
 *		type int and the result has type int.
 */

static
Type checkMultiplicative(Expression *&left, Expression *&right, const string &op)
{
    const Type &t1 = promote(left);
    const Type &t2 = promote(right);
    Type result = error;


    if (t1 != error && t2 != error) {
	if (t1.isInteger() && t2.isInteger())
	    result = t1;
	else
	    report(invalid_operands, op);
    }

    return result;
}


/*
 * Function:	checkMultiply
 *
 * Description:	Check a multiplication expression.
 */

Expression *checkMultiply(Expression *left, Expression *right)
{
    Type t = checkMultiplicative(left, right, "*");
    return new Multiply(left, right, t);
}


/*
 * Function:	checkDivide
 *
 * Description:	Check a division expression.
 */

Expression *checkDivide(Expression *left, Expression *right)
{
    Type t = checkMultiplicative(left, right, "/");
    return new Divide(left, right, t);
}


/*
 * Function:	checkRemainder
 *
 * Description:	Check a remainder expression.
 */

Expression *checkRemainder(Expression *left, Expression *right)
{
    Type t = checkMultiplicative(left, right, "%");
    return new Remainder(left, right, t);
}


/*
 * Function:	checkAdd
 *
 * Description:	Check an addition expression: if both operands have type
 *		int, then the result has type int; if one operand has a
 *		pointer type and other operand has type int, then the
 *		result has that pointer type.  Any pointer type must be
 *		complete.
 */

Expression *checkAdd(Expression *left, Expression *right)
{
    const Type &t1 = promote(left);
    const Type &t2 = promote(right);
    Type result = error;


    if (t1 != error && t2 != error) {
	if (isIncompletePointer(t1) || isIncompletePointer(t2))
	    report(incomplete_type);

	else if (t1.isInteger() && t2.isInteger())
	    result = t1;

	else if (t1.isPointer() && t2.isInteger()) {
	    right = scale(right, t1.deref().size());
	    result = t1;

	} else if (t1.isInteger() && t2.isPointer()) {
	    left = scale(left, t2.deref().size());
	    result = t2;

	} else
	    report(invalid_operands, "+");
    }

    return new Add(left, right, result);
}


/*
 * Function:	checkSubtract
 *
 * Description:	Check a subtraction expression: if both operands have
 *		type int, then the result has type int; if the left operand
 *		has a pointer type and the right operand has type int, then
 *		the result has that pointer type; if both operands are
 *		identical pointer types, then the result has type int.  Any
 *		pointer type must be complete.
 */

Expression *checkSubtract(Expression *left, Expression *right)
{
    Expression *tree;
    const Type &t1 = promote(left);
    const Type &t2 = promote(right);
    Type result = error;
    Type deref;


    if (t1 != error && t2 != error) {
	if (isIncompletePointer(t1) || isIncompletePointer(t2))
	    report(incomplete_type);

	else if (t1.isInteger() && t2.isInteger())
	    result = t1;

	else if (t1.isPointer() && t1 == t2)
	    result = integer;

	else if (t1.isPointer() && t2.isInteger()) {
	    right = scale(right, t1.deref().size());
	    result = t1;

	} else
	    report(invalid_operands, "-");
    }

    tree = new Subtract(left, right, result);

    if (t1.isPointer() && t1 == t2 && t1.deref().size() != 1)
	tree = new Divide(tree, new Number(t1.deref().size()), integer);

    return tree;
}


/*
 * Function:	checkComparative
 *
 * Description:	Check an equality or relational expression: the types of
 *		both operands must be compatible, and the result has type
 *		int.
 */

static Type checkComparative(Expression *&left, Expression *&right,
	const string &op)
{
    const Type &t1 = promote(left);
    const Type &t2 = promote(right);
    Type result = error;


    if (t1 != error && t2 != error) {
	if (t1.isCompatibleWith(t2))
	    result = integer;
	else
	    report(invalid_operands, op);
    }

    return result;
}


/*
 * Function:	checkEqual
 *
 * Description:	Check an equality expression: left == right.
 */

Expression *checkEqual(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, "==");
    return new Equal(left, right, t);
}


/*
 * Function:	checkNotEqual
 *
 * Description:	Check an inequality expression: left != right.
 */

Expression *checkNotEqual(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, "!=");
    return new NotEqual(left, right, t);
}


/*
 * Function:	checkLessThan
 *
 * Description:	Check a less-than expression: left < right.
 */

Expression *checkLessThan(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, "<");
    return new LessThan(left, right, t);
}


/*
 * Function:	checkGreaterThan
 *
 * Description:	Check a greater-than expression: left > right.
 */

Expression *checkGreaterThan(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, ">");
    return new GreaterThan(left, right, t);
}


/*
 * Function:	checkLessOrEqual
 *
 * Description:	Check a less-than-or-equal expression: left <= right.
 */

Expression *checkLessOrEqual(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, "<=");
    return new LessOrEqual(left, right, t);
}


/*
 * Function:	checkGreaterOrEqual
 *
 * Description:	Check a greater-than-or-equal expression: left >= right.
 */

Expression *checkGreaterOrEqual(Expression *left, Expression *right)
{
    Type t = checkComparative(left, right, ">=");
    return new GreaterOrEqual(left, right, t);
}


/*
 * Function:	checkLogical
 *
 * Description:	Check a logical-or or logical-and expression: the types of
 *		both operands must be value types and the result has type
 *		int.
 */

static
Type checkLogical(Expression *&left, Expression *&right, const string &op)
{
    const Type &t1 = promote(left);
    const Type &t2 = promote(right);
    Type result = error;


    if (t1 != error && t2 != error) {
	if (t1.isValue() && t2.isValue())
	    result = integer;
	else
	    report(invalid_operands, op);
    }

    return result;
}


/*
 * Function:	checkLogicalAnd
 *
 * Description:	Check a logical-and expression: left && right.
 */

Expression *checkLogicalAnd(Expression *left, Expression *right)
{
    Type t = checkLogical(left, right, "&&");
    return new LogicalAnd(left, right, t);
}


/*
 * Function:	checkLogicalOr
 *
 * Description:	Check a logical-or expression: left || right.
 */

Expression *checkLogicalOr(Expression *left, Expression *right)
{
    Type t = checkLogical(left, right, "||");
    return new LogicalOr(left, right, t);
}


/*
 * Function:	checkAssignment
 *
 * Description:	Check an assignment statement: the left operand must be an
 *		lvalue and the type of the operands must be compatible.
 */

Statement *checkAssignment(Expression *left, Expression *right)
{
    const Type &t1 = left->type();
    const Type &t2 = right->type(); // promote(right);


    if (t1 != error && t2 != error) {
	if (!left->lvalue())
	    report(invalid_lvalue);

	else if (!t1.isCompatibleWith(t2))
	    report(invalid_operands, "=");

	else if (t1 != t2)
	    promote(right);

	else if (t1 == character)
	    right = new Cast(right, t1);
    }

    return new Assignment(left, right);
}


/*
 * Function:	checkReturn
 *
 * Description:	Check a return statement: the type of the expression must
 *		be compatible with the given type, which should be the
 *		return type of the enclosing function.
 */

void checkReturn(Expression *&expr, const Type &type)
{
    const Type &t = promote(expr);

    if (t != error && !t.isCompatibleWith(type))
	report(invalid_return);
}


/*
 * Function:	checkTest
 *
 * Description:	Check if the type of the expression is a legal type in a
 * 		test expression in a while, if-then, if-then-else, or for
 * 		statement: the type must be a value type.
 */

void checkTest(Expression *&expr)
{
    const Type &t = promote(expr);

    if (t != error && !t.isValue())
	report(invalid_test);
}
