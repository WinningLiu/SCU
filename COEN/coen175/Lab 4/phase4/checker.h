/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include <string>
# include "Scope.h"

Scope *openScope();
Scope *closeScope();

void openStruct(const std::string &name);
void closeStruct(const std::string &name);

void declareSymbol(const std::string &name, const Type &type, bool = false);

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);

Type checkIndex(const Type &left, const Type &right);
Type checkCall(const Type &left, Parameters *p);
Type checkDirectStructRef(const Type &left, const std::string &id);
Type checkIndirectStructRef(const Type &left, const std::string &id);
Type checkLogicalOr(const Type &left, const Type &right);
Type checkLogicalAnd(const Type &left, const Type &right);
Type checkEqual(const Type &left, const Type &right);
Type checkNotEqual(const Type &left, const Type &right);
Type checkMul(const Type &left, const Type &right, const std::string &op);
Type checkNot(const Type &right);
Type checkNeg(const Type &right);
Type checkDeref(const Type &right);
Type checkAddr(const Type &right, const bool &lvalue);
Type checkSizeOf(const Type &right);
Type checkCast(const Type &right, const Type &type);
Type checkAdd(const Type &left, const Type &right, const std::string &op);
Type checkEqOrRelation(const Type &left, const Type &right, const std::string &op);
Type checkAssignment(const Type &left, const Type &right, const bool &lvalue);
void checkReturn (const Type &type, const Type &returnType);
void checkIf(const Type &type);

# endif /* CHECKER_H */
