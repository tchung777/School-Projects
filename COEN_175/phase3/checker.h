#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

using namespace std;
static void debug(const string &str, const Type &t1);
void openScope(string msg);
void closeScope(string msg);

Symbol* declareFunction(const string &name, const Type &type);
Symbol* defineFunction(const string &name, const Type &type);
Symbol* declareVariable(const string &name, const Type &type);
Symbol* callVariableFunction(const string &name, const Type &type);
Type checkVoid(const string &name, const Type &type);

#endif /* CHECKER_H */
