/*
* File:	checker.cpp
*
* Description:	This file contains the public and private function and
*		variable definitions for the semantic checker for Simple C.
*
*		If a symbol is redeclared, the existing declaration is
*		retained and the redeclaration discarded.  This behavior
*		seems to be consistent with GCC, and who are we to argue
*		with GCC?
*
*		Extra functionality:
*		- inserting an undeclared symbol with the error type
*/

# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "nullptr.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"

# define FUNCDEFN 1


using namespace std;

static Scope *outermost, *toplevel;
static const Type error, integer(INT), character(CHAR), voidPointer(VOID, 1);;

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";
static string void_object = "'%s' has type void";

static string invalid_break = "break statement not within loop";
static string invalid_return = "invalid return type";
static string invalid_test = "invalid type for test expression";
static string invalid_lvalue = "lvalue required in expression";
static string invalid_binary  = "invalid operands to binary %s";
static string invalid_unary = "invalid operand to unary %s";
static string invalid_function = "called object is not a function";
static string invalid_arguments = "invalid arguments to called function";

/*
* Function:	checkIfVoidObject
*
* Description:	Check if TYPE is a proper use of the void type (if the
*		specifier is void, then the indirection must be nonzero or
*		the kind must be a function).  If the type is proper, it is
*		returned.  Otherwise, the error type is returned.
*/

static Type checkIfVoidObject(const string name, const Type &type)
{
  if (type.specifier() != VOID)
  return type;

  if (type.indirection() == 0 && !type.isFunction()) {
    report(void_object, name);
    return error;
  }

  return type;
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
* Function:	defineFunction
*
* Description:	Define a function with the specified NAME and TYPE.  A
*		function is always defined in the outermost scope.
*/

Symbol *defineFunction(const string &name, const Type &type)
{
  Symbol *symbol = declareFunction(name, type);

  if (symbol->_attributes & FUNCDEFN)
  report(redefined, name);

  symbol->_attributes = FUNCDEFN;
  return symbol;
}


/*
* Function:	declareFunction
*
* Description:	Declare a function with the specified NAME and TYPE.  A
*		function is always declared in the outermost scope.  Any
*		redeclaration is discarded.
*/

Symbol *declareFunction(const string &name, const Type &type)
{
  cout << name << ": " << type << endl;
  Symbol *symbol = outermost->find(name);

  if (symbol == nullptr) {
    symbol = new Symbol(name, type);
    outermost->insert(symbol);

  } else if (type != symbol->type()) {
    report(conflicting, name);
    delete type.parameters();

  } else
  delete type.parameters();

  return symbol;
}


/*
* Function:	declareVariable
*
* Description:	Declare a variable with the specified NAME and TYPE.  Any
*		redeclaration is discarded.
*/

Symbol *declareVariable(const string &name, const Type &type)
{
  cout << name << ": " << type << endl;
  Symbol *symbol = toplevel->find(name);

  if (symbol == nullptr) {
    symbol = new Symbol(name, checkIfVoidObject(name, type));
    toplevel->insert(symbol);

  } else if (outermost != toplevel)
  report(redeclared, name);

  else if (type != symbol->type())
  report(conflicting, name);

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

Symbol *checkFunction(const string &name)
{
  Symbol *symbol = toplevel->lookup(name);

  if (symbol == nullptr)
    symbol = declareFunction(name, Type(INT, 0, nullptr));

  return symbol;
}

Type checkFuncCall(const Type &funcType, const Parameters &args){
    if(funcType == Type()){
        return Type();
    }
    Type t = Type();
    if(funcType.isFunction()){
        t = Type(funcType.specifier());
    }else{
        report(invalid_function);
        return Type();
    }

    Parameters *p = funcType.parameters();

    if (funcType.parameters()->_varargs == 0){
        //NO Ellipsis

        if(p != NULL){
          // If the function declaration’s parameter list does not end with an ellipsis, then the number of arguments and
          // parameters must agree and their types must be compatible [E8].
            if(funcType.parameters()->_types.size() == args._types.size()){
                for(unsigned i = 0; i < args._types.size(); i++){
                    if(args._types[i].isValue()){
                        if((*p)._types[i].isCompatibleWith(args._types[i])){  //is compatible
                            t = Type(funcType.specifier(), funcType.indirection());
                            return t;
                        }else{
                            report(invalid_arguments);
                            t = Type();
                        }
                    }else{
                        report(invalid_arguments);
                        t = Type();
                    }

                }
                return Type(funcType.specifier(), funcType.indirection());
            } else{
                report(invalid_arguments);
                return Type();
            }
        }
    }else {
        //Ellipsis
        if(p != NULL){
          // If the parameter list ends with an ellipsis, then
          // the number of arguments must exceed or equal the number of parameters, and the types of the arguments and
          // any corresponding parameters must be compatible [E8].
            if(funcType.parameters()->_types.size() == args._types.size()){
                for(unsigned i = 0; i < (*p)._types.size(); i++){
                    if(args._types[i].isValue()){
                        if((*p)._types[i].isCompatibleWith(args._types[i])){
                            t = Type(funcType.specifier(), funcType.indirection());
                            return t;
                        }else{
                            report(invalid_arguments);
                            t = Type();
                        }
                    }else{
                        report(invalid_arguments);
                        t = Type();
                    }
                }
                return Type(funcType.specifier(), funcType.indirection());
            }
        }
    }
    return t;

}

void *checkCall(const Symbol *id, std::vector<Type> &args){
    const Type &t = id->type();
    Type result = error;

    if (t != error) {
        if (!t.isFunction())
            report(invalid_function,"");

            else {
            Parameters *params = t.parameters();
            result = Type(t.specifier(), t.indirection());

            for (unsigned i = 0; i < args.size(); i ++)
                args[i] = args[i].promote();

            if (params != nullptr) {
            if (t.parameters()->_types.size() != args.size())
                report(invalid_arguments);

            else {
                for (unsigned i = 0; i < args.size(); i ++)
                if (!(*params)._types[i].isCompatibleWith(args[i])) {
                    report(invalid_arguments);
                    result = error;
                    break;
                }
            }
            }
        }
    }
}


Type checkLogical(const Type &left, const Type &right, const string &op) {
  if(left == error || right == error) {
    return error;
  }

  if(left.isValue() && right.isValue()) {
    return integer;
  }
  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkEquality(const Type &left, const Type &right, const string &op) {
  if(left == error || right == error) {
    return error;
  }
  if(left.isCompatibleWith(right)) {
    return integer;
  }
  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkRelational(const Type &left, const Type &right, const string &op) {
  if(left == error || right == error) {
    return error;
  }
  const Type &t1 = left.promote();
  const Type &t2 = right.promote();

  if(t1.isValue() && t1==t2) {
    return integer;
  }
  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkAdditiveExpression(const Type &left, const Type &right, const string &op) {
  /*
  If both operands have type int, then the result has type int. If the left operand has type “pointer to T,” where
  T is not void, and the right operand has type int, then the result has type “pointer to T.” For addition only, if
  the left operand has type int and the right operand has type “pointer to T,” where T is not void, then the result
  has type “pointer to T.” Otherwise, the result is an error [E5].
  In all cases, operands undergo type promotion and the result is never an lvalue.
  */

  const Type &t1 = left.promote();
  const Type &t2 = right.promote();

  if(t1 == error || t2 == error) {
    return error;
  }
  if(t1.isInteger() && t2.isInteger()) {
    return integer;
  }

  else if((t1.isPointer() && t1 != voidPointer) && t2.isInteger()) {
    return t1;
  }

  else if(t1.isInteger() && (t2 != voidPointer && t2.isPointer())) {
    return t2;
  }

  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkSubtractiveExpression(const Type &left, const Type &right, const string &op) {
  /*
  If both operands have type int, then the result has type int. If the left operand has type “pointer to T,” where
  T is not void, and the right operand has type int, then the result has type “pointer to T.”
  For subtraction only, if both operands have type “pointer to T,” where T is not void but
  is identical for both operands, then the result has type int. Otherwise, the result is an error [E5].
  In all cases, operands undergo type promotion and the result is never an lvalue.
  */

  const Type &t1 = left.promote();
  const Type &t2 = right.promote();

  if(t1 == error || t2 == error) {
    return error;
  }
  if(t1.isInteger() && t2.isInteger()) {
    return integer;
  }

  else if((t1.isPointer() && t1 != voidPointer) && t2.isInteger()) {
    return t1;
  }

  else if((t1.isPointer() && t1 != voidPointer) && t2 == t1) {
    return integer;
  }

  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkMultiplicativeExpression(const Type &left, const Type &right, const string &op) {
  // The types of both operands must be int, after any promotion [E5]. The result has type int and is not an lvalue.
  const Type &t1 = left.promote();
  const Type &t2 = right.promote();

  if(t1 == error || t2 == error) {
    return error;
  }
  if(t1.isInteger() && t2.isInteger()) {
    return left;
  }
  else {
    report(invalid_binary, op);
    return error;
  }
}

Type checkNegate(const Type &expr) {
  //The operand in a unary - expression must, after promotion, have type int [E6], and the result has type int
  const Type &t1 = expr.promote();
  if(t1 == error) {
    return error;
  }
  if(t1.isInteger()) {
    return integer;
  }
  else {
    report(invalid_unary, "-");
    return error;
  }
}

Type checkNot(const Type &expr) {
  //The operand in a ! expression must have a value type [E6], and the result has type int
  if(expr == error) {
    return error;
  }
  if(expr.isValue()) {
    return integer;
  }
  else {
    report(invalid_unary, "!");
    return error;
  }
}

Type checkDerefExpr(const Type &expr) {
  //The operand in a unary * expression must have type “pointer to T,” after any promotion and where T is not void [E6].
  //The result has type T and is an lvalue
  if(expr == Type()){ return Type(); }

  Type expr_type = expr.promote();

  if(expr_type.isPointer() && expr_type != voidPointer) {
    expr_type = Type(expr_type.specifier(), expr_type.indirection()-1);
    return expr_type;
  }
  else {
    report(invalid_unary, "*");
    return error;
  }
}

Type checkAddrExpr(const Type &expr, bool lvalue) {
  // The operand in a unary & expression must be an lvalue [E4].
  // If the operand has type T, then the result has type “pointer to T” and is not an lvalue.
  if(expr == error) {
    return error;
  }

  if(lvalue) {
    return Type(expr.specifier(), expr.indirection()+1);
  }
  else {
    report(invalid_lvalue, "&");
    return error;
  }
}

Type checkSizeofExpr(const Type &expr) {
  if(expr == error) {
    return error;
  }

  if(expr.isValue()) {
    return integer;
  }

  report(invalid_unary, "sizeof");
  return expr;
}

Type checkArray(const Type &left, const Type &right) {
  if(left == error || right == error) {
    return error;
  }
  Type left_type = left.promote();
  Type right_type = right.promote();

  if((left.isPointer() && left != voidPointer) && right.isInteger()) {
    return Type(left_type.specifier(), left_type.indirection()-1);
  }
  else {
    report(invalid_binary, "[]");
    return error;
  }
}

void checkReturn(const Type &expr, const Type &type)
{
  // The type of the expression in a return statement must be compatible with the return type of the enclosing function [E2].
  const Type &t = expr.promote();

  if (t != error && !t.isCompatibleWith(type))
    report(invalid_return);
}

void checkTest(const Type &expr)
{
  const Type &t = expr.promote();

  if (t != error && !t.isValue())
    report(invalid_test);
}

void checkBreak(int loop_counter){
  //A break statement must be within either a while or for statement [E1].
  if(loop_counter <= 0){
    report(invalid_break);
  }
}

Type checkAssignment(const Type &left, const Type &right, bool lvalue) {
  //if(left == error || right == error) {
  //  return error;
  //}
  if(left == error || right == error) {
    return error;
  }
  if(!lvalue) {
      report(invalid_lvalue);
  }
  else if (!left.isCompatibleWith(right)) {
	    report(invalid_binary, "=");
  }
  return left;
}
