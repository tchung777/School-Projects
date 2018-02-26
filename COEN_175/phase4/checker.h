/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);
Symbol *checkFunction(const std::string &name);

Type checkFuncCall(const Type &funcType, const Parameters &args);
void *checkCall(const Symbol *id, std::vector<Type> &args);
Type checkLogical(const Type &left, const Type &right, const std::string &op);
Type checkEquality(const Type &left, const Type &right, const std::string &op);
Type checkRelational(const Type &left, const Type &right, const std::string &op);
Type checkAdditiveExpression(const Type &left, const Type &right, const std::string &op);
Type checkSubtractiveExpression(const Type &left, const Type &right, const std::string &op);
Type checkMultiplicativeExpression(const Type &left, const Type &right, const std::string &op);
Type checkNegate(const Type &expr);
Type checkNot(const Type &expr);
Type checkDerefExpr(const Type &expr);
Type checkAddrExpr(const Type &expr, bool lvalue);
Type checkSizeofExpr(const Type &expr);
Type checkArray(const Type &left, const Type &right);
Type checkAssignment(const Type &left, const Type &right, bool lvalue);
void checkReturn(const Type &expr, const Type &type);
void checkTest(const Type &expr);
void checkBreak(int loop_counter);

# endif /* CHECKER_H */
