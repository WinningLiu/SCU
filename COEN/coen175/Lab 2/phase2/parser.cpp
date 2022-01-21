#include "lexer.h"
#include "tokens.h"
#include "string.h"
#include <iostream>
#include <cctype>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

// ./scc < hello.c > test_output.txt

using namespace std;

int lookahead;
string lexbuf;

void expression();
void statements();
void translation_unit_prime();
void pointers();
void global_or_function();
void declaration();
void declarations();
void parameter_list();
void parameter();
void declarator_list();
void logical_and_expression();
void expression_prime();
void expression_list_prime();
void multiplicative_expression_prime();
void additive_expression_prime();
void relational_expression_prime();
void equality_expression_prime();
void logical_and_expression_prime();
void global_declarator_list_prime();
void global_or_function_prime();
void global_declarator();
void assignment();
void equal_expression();
void declarator();
void parameter_list_prime();

bool isSpecifier(int t) {
    if (lookahead == STRUCT || lookahead == INT || lookahead == CHAR)
        return true;
    return false;
}

void error() {
    report("error at %s", lexbuf);
    exit(EXIT_FAILURE);
}

void match(int t) {
    if (lookahead == t)
        lookahead = lexan(lexbuf);
    else 
        error();
}

// done
void translation_unit() {
    if (lookahead == STRUCT) {
        match(STRUCT);
        match(ID);
        translation_unit_prime();
    }
    else if (lookahead == INT || lookahead == CHAR) {
        match(lookahead);
        pointers();
        global_or_function();
    }
}

// done
void translation_unit_prime() {
    if (lookahead == LBRACE) {
        match(LBRACE);
        declaration();
        declarations();
        match(RBRACE);
        match(SEMI);
        translation_unit();
    }
    else {
        pointers();
        global_or_function();
    }
}

// done
void specifier() {
    if (lookahead == STRUCT) {
        match(STRUCT);
        match(ID);
    }
    else if (lookahead == INT)
        match(INT);
    else if (lookahead == CHAR) 
        match(CHAR);
}

// done
void pointers() {
    if (lookahead == STAR) {
        match(STAR);
        pointers();
    }
}

// done
void parameters() {
    if (lookahead == VOID) 
        match(VOID);
    else
        parameter_list();
}

// check
void parameter_list() {
    parameter();
    parameter_list_prime();
}

// check
void parameter_list_prime() {
    if (lookahead == COMMA) {
        match(COMMA);
        parameter();
        parameter_list_prime();
    }
}

// done
void parameter() {
    specifier();
    pointers();
    if (lookahead == ID)
        match(ID);
    else if (lookahead == LPAREN) {
        match(LPAREN);
        match(STAR);
        match(ID);
        match(RPAREN);
        match(LPAREN);
        match(RPAREN);
    }
}

// done
void declarations() {
    if (lookahead == STRUCT || lookahead == INT || lookahead == CHAR) {
        declaration();
        declarations();
    }
}

// done
void declaration() {
    specifier();
    declarator_list();
    match(SEMI);
}

// done
void declarator_list() {
    declarator();
    if (lookahead == COMMA) {
        match(COMMA);
        declarator_list();
    }
}

// done
void declarator() {
    pointers();
    if (lookahead == ID) {
        match(ID);
        if (lookahead == LBRACK) {
            match(LBRACK);
            match(NUM);
            match(RBRACK);
        }
    }
    else if (lookahead == LPAREN) {
        match(LPAREN);
        match(STAR);
        match(ID);
        match(RPAREN);
        match(LPAREN);
        match(RPAREN);
    }
}

// done
void expression() {
    logical_and_expression();
    expression_prime();
}

// check again
void expression_prime() {
    if (lookahead == OR) {
        match(OR);
        logical_and_expression();
        cout << "or" << endl;
        expression_prime();
    }
}

// done
void expression_list() {
    expression();
    expression_list_prime();
}

void expression_list_prime() {
    if (lookahead == COMMA) {
        match(COMMA);
        expression();
        expression_list_prime();
    }
}

// done
void primary_expression(bool lp) {
    if (lp) {
        expression();
        match(RPAREN);
    }
    else if (lookahead == LPAREN) {
        match(LPAREN);
        expression();
        match(RPAREN);
    }
    else if (lookahead == ID)
        match(ID);
    else if (lookahead == CHAR)
        match(CHAR);
    else if (lookahead == STRING)
        match(STRING);
    else if (lookahead == NUM)
        match(NUM);
}

// done
void postfix_expression_prime() {
    if (lookahead == LBRACK) {
        match(LBRACK);
        expression();
        match(RBRACK);
        cout << "index" << endl;
        postfix_expression_prime();
    }
    else if (lookahead == LPAREN) {
        match(LPAREN);
        if (lookahead == RPAREN)
            match(RPAREN);
        else {
            expression_list();
            match(RPAREN);
        }
        cout << "call" << endl;
        postfix_expression_prime();
    }
    else if (lookahead == DOT) {
        match(DOT);
        match(ID);
        cout << "dot" << endl;
        postfix_expression_prime();
    }
    else if (lookahead == ARROW) {
        match(ARROW);
        match(ID);
        cout << "arrow" << endl;
        postfix_expression_prime();
    }
}

// done
void postfix_expression(bool lp) {
    primary_expression(lp);
    postfix_expression_prime();
}

// check again
void prefix_expression() {
    if (lookahead == NOT) {
        match(NOT);
        prefix_expression();
        cout << "not" << endl;
    }
    else if (lookahead == MINUS) {
        match(MINUS);
        prefix_expression();
        cout << "neg" << endl;
    }
    else if (lookahead == STAR) {
        match(STAR);
        prefix_expression();
        cout << "deref" << endl;
    }
    else if (lookahead == ADDR) {
        match(ADDR);
        prefix_expression();
        cout << "addr" << endl;
    }
    else if (lookahead == SIZEOF) {
        match(SIZEOF);
        if (lookahead == LPAREN) {
            match(LPAREN);
            if (isSpecifier(lookahead)) {
                specifier();
                pointers();
                match(RPAREN);
            }
            else
                postfix_expression(true);
        }
        else
            prefix_expression();
        cout << "sizeof" << endl;
    }
    else if (lookahead == LPAREN) {
        match(LPAREN);
        if (isSpecifier(lookahead)) {
            specifier();
            pointers();
            match(RPAREN);
            prefix_expression();
            cout << "cast" << endl;
        }
        else
            postfix_expression(true);
    }
    else
        postfix_expression(false);
}

// done
void multiplicative_expression () {
    prefix_expression();
    multiplicative_expression_prime();
}

// done
void multiplicative_expression_prime() {
    if (lookahead == STAR) {
        match(STAR);
        prefix_expression();
        cout << "mul" << endl;
        multiplicative_expression_prime();
    }
    else if (lookahead == DIV) {
        match(DIV);
        prefix_expression();
        cout << "div" << endl;
        multiplicative_expression_prime();
    }
    else if (lookahead == REM) {
        match(REM);
        prefix_expression();
        cout << "rem" << endl;
        multiplicative_expression_prime();
    }
}

// done
void additive_expression() {
    multiplicative_expression();
    additive_expression_prime();
}

// done
void additive_expression_prime() {
    if (lookahead == PLUS) {
        match(PLUS);
        multiplicative_expression();
        cout << "add" << endl;
        additive_expression_prime();
    }
    else if (lookahead == MINUS) {
        match(MINUS);
        multiplicative_expression();
        cout << "sub" << endl;
        additive_expression_prime();
    }
}

// done
void relational_expression() {
    additive_expression();
    relational_expression_prime();
}

// done
void relational_expression_prime() {
    if (lookahead == LTN) {
        match(LTN);
        additive_expression();
        cout << "ltn" << endl;
        relational_expression_prime();
    }
    else if (lookahead == GTN) {
        match(GTN);
        additive_expression();
        cout << "gtn" << endl;
        relational_expression_prime();      
    }
    else if (lookahead == LEQ) {
        match(LEQ);
        additive_expression();
        cout << "leq" << endl;
        relational_expression_prime();
    }
    else if (lookahead == GEQ) {
        match(GEQ);
        additive_expression();
        cout << "geq" << endl;
        relational_expression_prime();
    }
}

// done
void equality_expression() {
    relational_expression();
    equality_expression_prime();
}

// done
void equality_expression_prime() {
    if (lookahead == EQL) {
        match(EQL);
        relational_expression();
        cout << "eql" << endl;
        equality_expression_prime();
    }
    else if (lookahead == NEQ) {
        match(NEQ);
        relational_expression();
        cout << "neq" << endl;
        equality_expression_prime();   
    }
}

// done
void logical_and_expression() {
    equality_expression();
    logical_and_expression_prime();
}

// done
void logical_and_expression_prime() {
    if (lookahead == AND) {
        match(AND);
        equality_expression();
        cout << "and" << endl;
        logical_and_expression_prime();
    }
}

// done
void global_or_function() {
    if (lookahead == LPAREN) {
        match(LPAREN);
        match(STAR);
        match(ID);
        match(RPAREN);
        match(LPAREN);
        match(RPAREN);
        global_declarator_list_prime();
        match(SEMI);
        translation_unit();
    }
    else {
        match(ID);
        global_or_function_prime();
    }
}

// check again
void global_or_function_prime() {
    if (lookahead == LPAREN) {
        match(LPAREN);
        if (lookahead == RPAREN) {
            match(RPAREN);
            global_declarator_list_prime();
            match(SEMI);
            translation_unit();
        }
        else {
            parameters();
            match(RPAREN);
            match(LBRACE);
            declarations();
            statements();
            match(RBRACE);
            translation_unit();
        }
    }
    else if (lookahead == LBRACK) {
        match(LBRACK);
        match(NUM);
        match(RBRACK);
        global_declarator_list_prime();
        match(SEMI);
        translation_unit();
    }
    else {
        global_declarator_list_prime();
        match(SEMI);
        translation_unit();
    }
}

// done
void global_declarator_list_prime() {
    if (lookahead == COMMA) {
        match(COMMA);
        global_declarator();
        global_declarator_list_prime();
    }
}

// done
void global_declarator() {
    pointers();
    if (lookahead == LPAREN) {
        match(LPAREN);
        match(STAR);
        match(ID);
        match(RPAREN);
        match(LPAREN);
        match(RPAREN);
    }
    else {
        match(ID);
        if (lookahead == LPAREN) {
            match(LPAREN);
            match(RPAREN);
        }
        else if (lookahead == LBRACK) {
            match(LBRACK);
            match(NUM);
            match(RBRACK);
        }
    }
}

// done
void statement() {
    if (lookahead == LBRACE) {
        match(LBRACE);
        declarations();
        statements();
        match(RBRACE);
    }
    else if (lookahead == RETURN) {
        match(RETURN);
        expression();
        match(SEMI);
    }
    else if (lookahead == WHILE) {
        match(WHILE);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
    }
    else if (lookahead == FOR) {
        match(FOR);
        match(LPAREN);
        assignment();
        match(SEMI);
        expression();
        match(SEMI);
        assignment();
        match(RPAREN);
        statement();
    }
    else if (lookahead == IF) {
        match(IF);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
        if (lookahead == ELSE) {
            match(ELSE);
            statement();
        }
    }
    else {
        assignment();
        match(SEMI);
    }
}

// done
void assignment() {
    expression();
    equal_expression();
}

// done
void equal_expression() {
    if (lookahead == ASSIGN) {
        match(ASSIGN);
        expression();
    }
}

// done
void statements() {
    // statements only shows up twice, each time a RBRACE follows it
    if (lookahead != RBRACE) {
        statement();
        statements();
    }
}

int main() {
    lookahead = lexan(lexbuf);
    while (lookahead != DONE) {
        translation_unit();
        // expression();
        // statement();
        // declaration();
    }
}

// int main() {
//     string lexbuf;
//     while (true) {
//         int token = lexan(lexbuf);
//         cout << "TOKEN" << token << endl;
//         if (token == DONE)
//             break;
//     }
//     return 0;
// }