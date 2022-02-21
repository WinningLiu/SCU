/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "checker.h"
# include "tokens.h"
# include "lexer.h"

using namespace std;

static int lookahead;
static string lexbuf;

static Type expression(bool &lvalue);
static void statement(const Type &returnType);


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	error();

    lookahead = lexan(lexbuf);
}


/*
 * Function:	number
 *
 * Description:	Match the next token as a number and return its value.
 */

static unsigned number()
{
    string buf;


    buf = lexbuf;
    match(NUM);
    return strtoul(buf.c_str(), NULL, 0);
}


/*
 * Function:	identifier
 *
 * Description:	Match the next token as an identifier and return its name.
 */

static string identifier()
{
    string buf;


    buf = lexbuf;
    match(ID);
    return buf;
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    return token == INT || token == CHAR || token == STRUCT;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has int, char, and
 *		structure types.
 *
 *		specifier:
 *		  int
 *		  char
 *		  struct identifier
 */

static string specifier()
{
    if (lookahead == INT) {
	match(INT);
	return "int";
    }

    if (lookahead == CHAR) {
	match(CHAR);
	return "char";
    }

    match(STRUCT);
    return identifier();
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

static unsigned pointers()
{
    unsigned count = 0;


    while (lookahead == '*') {
	match('*');
	count ++;
    }

    return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, both with optional pointer
 *		declarators, or a callback (i.e., a simple function
 *		pointer).
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ num ]
 *		  pointers ( * identifier ) ( )
 */

static void declarator(const string &typespec)
{
    unsigned indirection;
    string name;


    indirection = pointers();

    if (lookahead == '(') {
	match('(');
	match('*');
	name = identifier();
	declareSymbol(name, Callback(typespec, indirection));
	match(')');
	match('(');
	match(')');

    } else {
	name = identifier();

	if (lookahead == '[') {
	    match('[');
	    declareSymbol(name, Array(typespec, indirection, number()));
	    match(']');
	} else
	    declareSymbol(name, Scalar(typespec, indirection));
    }
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ';'
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
    string typespec;


    typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
	match(',');
	declarator(typespec);
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
	declaration();
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier
 *		  character
 *		  string
 *		  num
 */

static Type primaryExpression(bool lparen, bool &lvalue)
{
    if (lparen) {
		Type right = expression(lvalue);
		match(')');
		return right;
    } 
	// char to int
	else if (lookahead == CHARACTER) {
		match(CHARACTER);
		Type characterType = Scalar("int");
		lvalue = false;
		return characterType;
	}

    else if (lookahead == STRING) {
		match(STRING);
		Type stringType = Array("char", 0, lexbuf.size() - 2);
		lvalue = false;
		return stringType;
	}

    else if (lookahead == NUM) {
		match(NUM);	
		Type numType = Scalar("int");
		lvalue = false;
		return numType;
	}

    else if (lookahead == ID) {
		Symbol *symbol = checkIdentifier(identifier());
		if (symbol->type().isCallback() == true || symbol->type().isScalar() == true)
			lvalue = true;
		else
			lvalue = false;
		return symbol->type();
	}

    else {
		error();
		return Type();
	}
	
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 *		  postfix-expression ( expression-list )
 *		  postfix-expression ( )
 *		  postfix-expression . identifier
 *		  postfix-expression -> identifier
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static Type postfixExpression(bool lparen, bool &lvalue)
{
    Type left = primaryExpression(lparen, lvalue);
	bool tempLvalue = false;

    while (1) {
	if (lookahead == '[') {
	    match('[');
	   	Type right = expression(tempLvalue);
		lvalue = true;
		left = checkIndex(left, right);
	    match(']');
	    cout << "index" << endl;

	} else if (lookahead == '(') {
	    match('(');
		Parameters *param = new Parameters();
	    if (lookahead != ')') {
		param->push_back(expression(tempLvalue));

		while (lookahead == ',') {
		    match(',');
		    param->push_back(expression(tempLvalue));
		}
	    }

		lvalue = false;
		left = checkCall(left, param);
		delete(param);

	    match(')');
	    cout << "call" << endl;

	} else if (lookahead == '.') {
	    match('.');
	    string id = identifier();
		left = checkDirectStructRef(left, id);
		lvalue = !(left.isArray()) && lvalue;
	    cout << "dot" << endl;

	} else if (lookahead == ARROW) {
	    match(ARROW);
	    string id = identifier();
		left = checkIndirectStructRef(left, id);
		lvalue = !(left.isArray());
	    cout << "arrow" << endl;

	} else
	    break;
    }

	return left;
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof prefix-expression
 *		  sizeof ( specifier pointers )
 *		  ( specifier pointers ) prefix-expression
 *
 *		This grammar is still ambiguous since "sizeof(type) * n"
 *		could be interpreted as a multiplicative expression or as a
 *		cast of a dereference.  The correct interpretation is the
 *		former, as the latter makes little sense semantically.  We
 *		resolve the ambiguity here by always consuming the "(type)"
 *		as part of the sizeof expression.
 */

// Finished
static Type prefixExpression(bool &lvalue)
{
    if (lookahead == '!') {
	match('!');
	Type right = prefixExpression(lvalue);
	cout << "check !" << endl;
	right = checkNot(right);
	lvalue = false;
	return right;

    } else if (lookahead == '-') {
	match('-');
	Type right = prefixExpression(lvalue);
	cout << "check neg" << endl;
	right = checkNeg(right);
	lvalue = false;
	return right;

    } else if (lookahead == '*') {
	match('*');
	Type right = prefixExpression(lvalue);
	cout << "deref" << endl;
	right = checkDeref(right);
	lvalue = true;
	return right;

    } else if (lookahead == '&') {
	match('&');
	Type right = prefixExpression(lvalue);
	cout << "addr" << endl;
	right = checkAddr(right, lvalue);
	lvalue = false;
	return right;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);

	if (lookahead == '(') {
	    match('(');

	    if (isSpecifier(lookahead)) {
		string spec = specifier();
		unsigned indirection = pointers();
		match(')');
		Type scalar = Scalar(spec, indirection);
		Type right = checkSizeOf(scalar);
		return right;
	    } else {
			Type right = postfixExpression(true, lvalue);
			right = checkSizeOf(right);
			lvalue = false;
			return right;
		}
		
	} else {
		Type right = prefixExpression(lvalue);
		right = checkSizeOf(right);
		lvalue = false;
		return right;
	}
	    
	cout << "sizeof" << endl;

    } else if (lookahead == '(') {
	match('(');

	if (isSpecifier(lookahead)) {
	    string spec = specifier();
	    unsigned indirection = pointers();
	    match(')');
	    Type right = prefixExpression(lvalue);
		right = checkCast(right, Scalar(spec, indirection));
		lvalue = false;
	    cout << "cast" << endl;
		return right;
	} else {
		Type right = postfixExpression(true, lvalue);
		return right;
	}

    } else {
		Type right = postfixExpression(false, lvalue);
		return right;
	}
		
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

// Finished
static Type multiplicativeExpression(bool &lvalue)
{
    Type left = prefixExpression(lvalue);

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    Type right = prefixExpression(lvalue);
		left = checkMul(left, right, "*");
		lvalue = false;
	    cout << "mul" << endl;

	} else if (lookahead == '/') {
	    match('/');
	    Type right = prefixExpression(lvalue);
		left = checkMul(left, right, "/");
		lvalue = false;
	    cout << "div" << endl;

	} else if (lookahead == '%') {
	    match('%');
	    Type right = prefixExpression(lvalue);
		left = checkMul(left, right, "%");
		lvalue = false;
	    cout << "rem" << endl;
		

	} else
	    return left;
    }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

// Finished
static Type additiveExpression(bool &lvalue)
{
    Type left = multiplicativeExpression(lvalue);

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    Type right = multiplicativeExpression(lvalue);
		left = checkAdd(left, right, "+");
		lvalue = false;
	    cout << "add" << endl;

	} else if (lookahead == '-') {
	    match('-');
	    Type right = multiplicativeExpression(lvalue);
		left = checkAdd(left, right, "-");
		lvalue = false;
	    cout << "sub" << endl;
		

	} else
	    return left;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static Type relationalExpression(bool &lvalue)
{
    Type left = additiveExpression(lvalue);

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    Type right = additiveExpression(lvalue);
		left = checkEqOrRelation(left, right, "<");
	    cout << "ltn" << endl;
		lvalue = false;


	} else if (lookahead == '>') {
	    match('>');
	    Type right = additiveExpression(lvalue);
		left = checkEqOrRelation(left, right, ">");
	    cout << "gtn" << endl;
		lvalue = false;

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    Type right = additiveExpression(lvalue);
		left = checkEqOrRelation(left, right, "<=");
	    cout << "leq" << endl;
		lvalue = false;

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    Type right = additiveExpression(lvalue);
		left = checkEqOrRelation(left, right, ">=");
	    cout << "geq" << endl;
		lvalue = false;
		

	} else
	    return left;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

// Finished
static Type equalityExpression(bool &lvalue)
{
    Type left = relationalExpression(lvalue);

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    Type right = relationalExpression(lvalue);
		left = checkEqual(left, right);
	    cout << "eql" << endl;
		lvalue = false;

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    Type right = relationalExpression(lvalue);
		left = checkNotEqual(left, right);
	    cout << "neq" << endl;
		lvalue = false;

	} else
		return left;  
    }
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

//Finished
static Type logicalAndExpression(bool &lvalue)
{
	Type left = equalityExpression(lvalue);

    while (lookahead == AND) {
		match(AND);
		Type right = equalityExpression(lvalue);
		left = checkLogicalAnd(left, right);
		lvalue = false;
		cout << "check &&" << endl;
    }
	
	return left;
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

// Finished
Type expression(bool &lvalue)
{
    Type left = logicalAndExpression(lvalue);

    while (lookahead == OR) {
		match(OR);
		Type right = logicalAndExpression(lvalue);
		cout << "check ||" << endl;
		left = checkLogicalOr(left, right);
		lvalue = false;
    }
	return left;
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements(const Type &returnType)
{
    while (lookahead != '}')
	statement(returnType);
}


/*
 * Function:	assignment
 *
 * Description:	Parse an assignment statement.
 *
 *		assignment:
 *		  expression = expression
 *		  expression
 */

static void assignment()
{
	bool lvalue = false;
    Type left = expression(lvalue);

    if (lookahead == '=') {
		match('=');
		bool lvalue1 = lvalue;
		Type right = expression(lvalue1);
		left = checkAssignment(left, right, lvalue);
    }
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  return expression ;
 *		  while ( expression ) statement
 *		  for ( assignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */

//Finished
static void statement(const Type &returnType)
{
	bool lvalue = false;
    if (lookahead == '{') {
	match('{');
	openScope();
	declarations();
	statements(returnType);
	closeScope();
	match('}');

    } else if (lookahead == RETURN) {
	match(RETURN);
	Type type = expression(lvalue);
	checkReturn(type, returnType);
	match(';');

    } else if (lookahead == WHILE) {
	match(WHILE);
	match('(');
	Type type = expression(lvalue);
	checkIf(type);
	match(')');
	statement(returnType);

    } else if (lookahead == FOR) {
	match(FOR);
	match('(');
	assignment();
	match(';');
	Type type = expression(lvalue);
	checkIf(type);
	match(';');
	assignment();
	match(')');
	statement(returnType);

    } else if (lookahead == IF) {
	match(IF);
	match('(');
	Type type = expression(lvalue);
	checkIf(type);
	match(')');
	statement(returnType);

	if (lookahead == ELSE) {
	    match(ELSE);
	    statement(returnType);
	}

    } else {
	assignment();
	match(';');
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always either a
 *		simple variable with optional pointer declarators, or a
 *		callback (i.e., a simple function pointer)
 *
 *		parameter:
 *		  specifier pointers identifier
 *		  specifier pointers ( * identifier ) ( )
 */

static Type parameter()
{
    unsigned indirection;
    string typespec, name;
    Type type;


    typespec = specifier();
    indirection = pointers();

    if (lookahead == '(') {
	match('(');
	match('*');
	name = identifier();
	type = Callback(typespec, indirection);
	declareSymbol(name, type, true);
	match(')');
	match('(');
	match(')');

    } else {
	name = identifier();
	type = Scalar(typespec, indirection);
	declareSymbol(name, type, true);
    }

    return type;
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  parameter-list
 *
 *		parameter-list:
 *		  parameter
 *		  parameter , parameter-list
 */

static Parameters *parameters()
{
    Parameters *params;


    params = new Parameters();

    if (lookahead == VOID)
	match(VOID);

    else {
	params->push_back(parameter());

	while (lookahead == ',') {
	    match(',');
	    params->push_back(parameter());
	}
    }

    return params;
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, all with optional
 *		pointer	declarators, or a callback (i.e., a simple function
 *		pointer).
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier ( )
 *		  pointers identifier [ num ]
 *		  pointers ( * identifier ) ( )
 */

static void globalDeclarator(const string &typespec)
{
    unsigned indirection;
    string name;


    indirection = pointers();

    if (lookahead == '(') {
	match('(');
	match('*');
	name = identifier();
	declareSymbol(name, Callback(typespec, indirection));
	match(')');
	match('(');
	match(')');

    } else {
	name = identifier();

	if (lookahead == '(') {
	    match('(');
	    declareSymbol(name, Function(typespec, indirection));
	    match(')');

	} else if (lookahead == '[') {
	    match('[');
	    declareSymbol(name, Array(typespec, indirection, number()));
	    match(']');

	} else
	    declareSymbol(name, Scalar(typespec, indirection));
    }
}


/*
 * Function:	remainingDeclarators
 *
 * Description:	Parse any remaining global declarators after the first.
 *
 * 		remaining-declarators:
 * 		  ;
 * 		  , global-declarator remaining-declarators
 */

static void remainingDeclarators(const string &typespec)
{
    while (lookahead == ',') {
	match(',');
	globalDeclarator(typespec);
    }

    match(';');
}


/*
 * Function:	globalOrFunction
 *
 * Description:	Parse a global declaration or function definition.
 *
 * 		global-or-function:
 * 		  struct identifier { declaration declarations } ;
 * 		  specifier pointers identifier remaining-decls
 * 		  specifier pointers identifier ( ) remaining-decls 
 * 		  specifier pointers identifier [ num ] remaining-decls
 * 		  specifier pointers identifier ( parameters ) { ... }
 */

static void globalOrFunction()
{
    unsigned indirection;
    string typespec, name;
    Type type;


    typespec = specifier();

    if (typespec != "int" && typespec != "char" && lookahead == '{') {
	openStruct(typespec);
	match('{');
	declaration();
	declarations();
	closeStruct(typespec);
	match('}');
	match(';');

    } else {
	indirection = pointers();

	if (lookahead == '(') {
	    match('(');
	    match('*');
	    name = identifier();
	    declareSymbol(name, Callback(typespec, indirection));
	    match(')');
	    match('(');
	    match(')');
	    remainingDeclarators(typespec);

	} else {
	    name = identifier();

	    if (lookahead == '[') {
		match('[');
		declareSymbol(name, Array(typespec, indirection, number()));
		match(']');
		remainingDeclarators(typespec);

	    } else if (lookahead == '(') {
		match('(');

		if (lookahead == ')') {
		    declareSymbol(name, Function(typespec, indirection));
		    match(')');
		    remainingDeclarators(typespec);

		} else {
		    openScope();
		    type = Function(typespec, indirection, parameters());
		    defineFunction(name, type);
		    match(')');
		    match('{');
		    declarations();
		    statements(type);
		    closeScope();
		    match('}');
		}

	    } else {
		declareSymbol(name, Scalar(typespec, indirection));
		remainingDeclarators(typespec);
	    }
	}
    }
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    openScope();
    lookahead = lexan(lexbuf);

    while (lookahead != DONE)
	globalOrFunction();

    closeScope();
    exit(EXIT_SUCCESS);
}
