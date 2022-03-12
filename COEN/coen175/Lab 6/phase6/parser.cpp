/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "generator.h"
# include "checker.h"
# include "tokens.h"
# include "string.h"
# include "lexer.h"
# include "Tree.h"

using namespace std;

static int lookahead;
static string lexbuf;

static Expression *expression();
static Statement *statement();
static Type returnType;


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

static Expression *primaryExpression(bool lparen)
{
    Expression *expr;


    if (lparen) {
	expr = expression();
	match(')');

    } else if (lookahead == CHARACTER) {
	lexbuf = lexbuf.substr(1, lexbuf.size() - 2);
	expr = new Number(parseString(lexbuf)[0]);
	match(CHARACTER);

    } else if (lookahead == STRING) {
	lexbuf = lexbuf.substr(1, lexbuf.size() - 2);
	expr = new String(parseString(lexbuf));
	match(STRING);

    } else if (lookahead == NUM) {
	expr = new Number(lexbuf);
	match(NUM);

    } else if (lookahead == ID) {
	expr = new Identifier(checkIdentifier(identifier()));

    } else {
	expr = nullptr;
	error();
    }

    return expr;
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

static Expression *postfixExpression(bool lparen)
{
    Expression *left, *right;


    left = primaryExpression(lparen);

    while (1) {
	if (lookahead == '[') {
	    match('[');
	    right = expression();
	    left = checkArray(left, right);
	    match(']');

	} else if (lookahead == '(') {
	    match('(');
	    Expressions args;

	    if (lookahead != ')') {
		args.push_back(expression());

		while (lookahead == ',') {
		    match(',');
		    args.push_back(expression());
		}
	    }

	    left = checkCall(left, args);
	    match(')');

	} else if (lookahead == '.') {
	    match('.');
	    left = checkDirectField(left, identifier());

	} else if (lookahead == ARROW) {
	    match(ARROW);
	    left = checkIndirectField(left, identifier());

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

static Expression *prefixExpression()
{
    Expression *expr;
    unsigned indirection;
    string typespec;


    if (lookahead == '!') {
	match('!');
	expr = prefixExpression();
	expr = checkNot(expr);

    } else if (lookahead == '-') {
	match('-');
	expr = prefixExpression();
	expr = checkNegate(expr);

    } else if (lookahead == '*') {
	match('*');
	expr = prefixExpression();
	expr = checkDereference(expr);

    } else if (lookahead == '&') {
	match('&');
	expr = prefixExpression();
	expr = checkAddress(expr);

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);

	if (lookahead == '(') {
	    match('(');

	    if (isSpecifier(lookahead)) {
		typespec = specifier();
		indirection = pointers();
		expr = checkSizeof(Scalar(typespec, indirection));
		match(')');
	    } else {
		expr = postfixExpression(true);
		expr = checkSizeof(expr->type());
	    }

	} else {
	    expr = prefixExpression();
	    expr = checkSizeof(expr->type());
	}

    } else if (lookahead == '(') {
	match('(');

	if (isSpecifier(lookahead)) {
	    typespec = specifier();
	    indirection = pointers();
	    match(')');
	    expr = prefixExpression();
	    expr = checkCast(Scalar(typespec, indirection), expr);
	} else
	    expr = postfixExpression(true);

    } else
	expr = postfixExpression(false);

    return expr;
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

static Expression *multiplicativeExpression()
{
    Expression *left, *right;


    left = prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    right = prefixExpression();
	    left = checkMultiply(left, right);

	} else if (lookahead == '/') {
	    match('/');
	    right = prefixExpression();
	    left = checkDivide(left, right);

	} else if (lookahead == '%') {
	    match('%');
	    right = prefixExpression();
	    left = checkRemainder(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *additiveExpression()
{
    Expression *left, *right;


    left = multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    right = multiplicativeExpression();
	    left = checkAdd(left, right);

	} else if (lookahead == '-') {
	    match('-');
	    right = multiplicativeExpression();
	    left = checkSubtract(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *relationalExpression()
{
    Expression *left, *right;


    left = additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    right = additiveExpression();
	    left = checkLessThan(left, right);

	} else if (lookahead == '>') {
	    match('>');
	    right = additiveExpression();
	    left = checkGreaterThan(left, right);

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    right = additiveExpression();
	    left = checkLessOrEqual(left, right);

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    right = additiveExpression();
	    left = checkGreaterOrEqual(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *equalityExpression()
{
    Expression *left, *right;


    left = relationalExpression();

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    right = relationalExpression();
	    left = checkEqual(left, right);

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    right = relationalExpression();
	    left = checkNotEqual(left, right);

	} else
	    break;
    }

    return left;
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

static Expression *logicalAndExpression()
{
    Expression *left, *right;


    left = equalityExpression();

    while (lookahead == AND) {
	match(AND);
	right = equalityExpression();
	left = checkLogicalAnd(left, right);
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

static Expression *expression()
{
    Expression *left, *right;


    left = logicalAndExpression();

    while (lookahead == OR) {
	match(OR);
	right = logicalAndExpression();
	left = checkLogicalOr(left, right);
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

static Statements statements()
{
    Statements stmts;


    while (lookahead != '}')
	stmts.push_back(statement());

    return stmts;
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

static Statement *assignment()
{
    Expression *expr;


    expr = expression();

    if (lookahead == '=') {
	match('=');
	return checkAssignment(expr, expression());
    }

    return new Simple(expr);
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

static Statement *statement()
{
    Scope *decls;
    Expression *expr;
    Statement *stmt, *init, *incr;
    Statements stmts;


    if (lookahead == '{') {
	match('{');
	openScope();
	declarations();
	stmts = statements();
	decls = closeScope();
	match('}');
	return new Block(decls, stmts);
    }
    
    if (lookahead == RETURN) {
	match(RETURN);
	expr = expression();
	checkReturn(expr, returnType);
	match(';');
	return new Return(expr);
    }
    
    if (lookahead == WHILE) {
	match(WHILE);
	match('(');
	expr = expression();
	checkTest(expr);
	match(')');
	stmt = statement();
	return new While(expr, stmt);
    }
    
    if (lookahead == FOR) {
	match(FOR);
	match('(');
	init = assignment();
	match(';');
	expr = expression();
	checkTest(expr);
	match(';');
	incr = assignment();
	match(')');
	stmt = statement();
	return new For(init, expr, incr, stmt);
    }
    
    if (lookahead == IF) {
	match(IF);
	match('(');
	expr = expression();
	checkTest(expr);
	match(')');
	stmt = statement();

	if (lookahead != ELSE)
	    return new If(expr, stmt, nullptr);

	match(ELSE);
	return new If(expr, stmt, statement());
    }

    stmt = assignment();
    match(';');
    return stmt;
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
    Statements stmts;
    Procedure *proc;
    Symbol *symbol;
    Scope *decls;
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
		    returnType = Scalar(typespec, indirection);
		    symbol = defineFunction(name, type);
		    match(')');
		    match('{');
		    declarations();
		    stmts = statements();
		    decls = closeScope();
		    proc = new Procedure(symbol, new Block(decls, stmts));
		    match('}');

		    if (numerrors == 0)
			proc->generate();
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

    generateGlobals(closeScope());
    exit(EXIT_SUCCESS);
}
