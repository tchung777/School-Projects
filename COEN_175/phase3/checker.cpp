# include <iostream>
# include <string>
# include "checker.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"
# include "lexer.h"
# include "tokens.h"

using namespace std;

static Scope *globalScope, *currentScope;
static string error1 = "conflicting types for '%s'";
static string error2 = "redefinition of '%s'";
static string error3 = "redeclaration of '%s'";
static string error4 = "'%s' undeclared";
static string error5 = "'%s' has type void";
static Type error;

static void debug(const string &str, const Type &t1)
{
    ;//cout << "line " << yylineno << ": " << str << " " << t1 << endl;
}

void openScope(string msg)
{
  //cout << msg << endl;
  currentScope = new Scope(currentScope);

  //If this is the first scope we're encountering, then this is the global scope.
  if (globalScope == NULL)
    globalScope = currentScope;

}


void closeScope(string msg)
{
    //cout << msg << endl;
    //currentScope->printAll();
    currentScope = currentScope->enclosing();
}


Symbol* declareFunction(const string &name, const Type &type)
{
    //Look for the declaration in the GLOBAL scope.
    Symbol *symFunc = globalScope->find(name);

    //If it hasn't been declared yet.
    if(symFunc == nullptr) {

      //Create a new symbol for the function.
      symFunc = new Symbol(name, type);

      //Update our symbol table for our current scope.
      globalScope->insert(symFunc);

      //globalScope->printAll();
    } else if(type != symFunc->getType()) {
      report(error1, name);
      delete type.parameters();

    } else
    delete type.parameters();

    return symFunc;
}

Symbol* defineFunction(const string &name, const Type &type)
{
  debug(name, type);
  //Check if the symbol has been declared yet.
  Symbol *symFunc = declareFunction(name, type);

  //If this function has already been called on the symbol, then we're re-defining which is a no-no.
  if(symFunc->_attributes != 0)
    report(error2, name);
  //Update this symbols' attributes to denote that it has been defined.
  symFunc->_attributes = 1;
  return symFunc;
}

Symbol* declareVariable(const string &name, const Type &type) {
  debug(name, type);

  //See if this name hasn't been used yet in the current scope.
  Symbol *symVar = currentScope->find(name);
  if(symVar == nullptr) {
    symVar = new Symbol(name, checkVoid(name, type)); //Create new symbol
    currentScope->insert(symVar); //Update scope's symbol table.
  } else if(globalScope != currentScope) {
    report(error3, name); //If the variable is a local variable, then the variable must not be previously declared in the current scope [E3].
  } else if(symVar->getType() != type)
    report(error1, name); //If the variable is a global variable, then any previous declaration must be identical [E1].

  return symVar;
}

Type checkVoid(const string &name, const Type &type) {
  //If not void, return type
  if(type.specifier() != VOID) return type;

  if(type.indirection() == 0 && !type.isFunction()) {
    report(error5, name); //  A variable of type void or “array of void” is illegal [E5].
    return error;
  }

  return type; // Variables of type “pointer to void” or “array of pointer to void” are examples of legal declarations.
}

Symbol *callVariableFunction(const string &name, const Type &type)
{
	Symbol *symbol;
	symbol = currentScope->lookup(name);
	if(symbol == NULL){
		if(type.isFunction()) // FUNCTION=2
			declareFunction(name, type);
		else {
      report(error4, name);
      symbol = new Symbol(name, error);
	    currentScope->insert(symbol);
    }
	}

	return symbol;
}
