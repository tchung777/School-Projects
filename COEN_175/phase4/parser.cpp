/*
* File:	parser.c
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
static Type returnType;
static int loop_counter = 0;
static Type expression(bool &lvalue);
static void statement();

void debugLog(const string &op) {
  ;//cout << "Line: " << yylineno << " " << op << endl;
}

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
  report("syntax error at '%s'", yytext);

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

  lookahead = yylex();
}


/*
* Function:	expect
*
* Description:	Match the next token against the specified token, and
*		return its lexeme.  We must save the contents of the buffer
*		from the lexical analyzer before matching, since matching
*		will advance to the next token.
*/

static string expect(int t)
{
  string buf = yytext;
  match(t);
  return buf;
}


/*
* Function:	number
*
* Description:	Match the next token as a number and return its value.
*/

static unsigned number()
{
  int value;


  value = strtoul(expect(NUM).c_str(), NULL, 0);
  return value;
}


/*
* Function:	isSpecifier
*
* Description:	Return whether the given token is a type specifier.
*/

static bool isSpecifier(int token)
{
  return token == INT || token == CHAR || token == VOID;
}


/*
* Function:	specifier
*
* Description:	Parse a type specifier.  Simple C has only ints, chars, and
*		void types.
*
*		specifier:
*		  int
*		  char
*		  void
*/

static int specifier()
{
  int typespec = ERROR;


  if (isSpecifier(lookahead)) {
    typespec = lookahead;
    match(lookahead);
  } else
  error();

  return typespec;
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
*		variable or an array, with optional pointer declarators.
*
*		declarator:
*		  pointers identifier
*		  pointers identifier [ num ]
*/

static void declarator(int typespec)
{
  unsigned indirection;
  string name;


  indirection = pointers();
  name = expect(ID);

  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typespec, indirection, number()));
    match(']');
  } else
  declareVariable(name, Type(typespec, indirection));
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
  int typespec;


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
*		  identifier ( expression-list )
*		  identifier ( )
*		  identifier
*		  character
*		  string
*		  num
*
*		expression-list:
*		  expression
*		  expression , expression-list
*/

static Type primaryExpression(bool &lvalue)
{
  Type expr;
  string name;
  Parameters parameters;
  Symbol *function_symbol;

  if (lookahead == '(') {
    //The type of a parenthesized expression is that of the expression,
    // and is an lvalue if the expression itself is an lvalue.
    match('(');
    expr = expression(lvalue);
    match(')');
    lvalue = false;

  } else if (lookahead == CHARACTER) {
    //A character literal has type int and is not an lvalue
    expect(CHARACTER);
    expr = Type(INT, 0);
    lvalue = false;

  } else if (lookahead == STRING) {
    //A string literal has type “array of char” and is not an lvalue
    string buf_length = expect(STRING);
    expr = Type(CHAR, 0, buf_length.size() - 1);
    lvalue = false;

  } else if (lookahead == NUM) {
    //A number has type int and is not an lvalue.
    expect(NUM);
    expr = Type(INT);
    lvalue = false;

  } else if (lookahead == ID) {
		name = expect(ID);
		// checkIdentifier(expect(ID));
		lvalue=false;

    // The identifier in a function call expression must have type “function returning T” and the result has type T. In
    // addition, the arguments undergo promotion and must be value types. The result is not an lvalue.
		if (lookahead == '(') {
			match('(');

			if (lookahead != ')') {
				parameters._types.push_back(expression(lvalue));

				while (lookahead == ',') {
					match(',');
					parameters._types.push_back(expression(lvalue));
				}
			}

			match(')');
			function_symbol = checkFunction(name);
			expr = checkFuncCall(function_symbol->type(),parameters);
			return expr;
    }else {
      Symbol *checkID = checkIdentifier(name);
      expr = checkID->type();
      if(expr.isScalar()){
        lvalue = true;
      }
      else{
        lvalue = false;
      }
      return expr;
		}

  } else {
    error();
    expr = Type();
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
*/

static Type postfixExpression(bool &lvalue)
{
  //postfixExpr have two operand
  Type left = primaryExpression(lvalue);

  while (lookahead == '[') {
    match('[');
    Type right = expression(lvalue);
    left = checkArray(left,right);
    match(']');
    lvalue = true;
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
*/

static Type prefixExpression(bool &lvalue)
{
  //prefixExpr only have one operand
  Type expr;
  if (lookahead == '!') {
    match('!');
    expr = prefixExpression(lvalue);
    expr = checkNot(expr);
    lvalue = false;

  } else if (lookahead == '-') {
    match('-');
    expr = prefixExpression(lvalue);
    expr = checkNegate(expr);
    lvalue = false;

  } else if (lookahead == '*') {
    match('*');
    expr = prefixExpression(lvalue);
    expr = checkDerefExpr(expr);
    lvalue = true;

  } else if (lookahead == '&') {
    match('&');
    expr = prefixExpression(lvalue);
    expr = checkAddrExpr(expr, lvalue);
    lvalue = false;

  } else if (lookahead == SIZEOF) {
    match(SIZEOF);
    expr = prefixExpression(lvalue);
    expr = checkSizeofExpr(expr);
    lvalue = false;

  } else
    expr = postfixExpression(lvalue);
  return expr;
}


/*
* Function:	multiplicativeExpression
*
* Description:	Parse a multiplicative expression.  Simple C does not have
*		cast expressions, so we go immediately to prefix
*		expressions.
*
*		multiplicative-expression:
*		  prefix-expression
*		  multiplicative-expression * prefix-expression
*		  multiplicative-expression / prefix-expression
*		  multiplicative-expression % prefix-expression
*/

static Type multiplicativeExpression(bool &lvalue)
{
  Type left = prefixExpression(lvalue);

  while (1) {
    if (lookahead == '*') {
      match('*');
      Type right = prefixExpression(lvalue);
      left = checkMultiplicativeExpression(left, right,"*");
      lvalue = false;

    } else if (lookahead == '/') {
      match('/');
      Type right = prefixExpression(lvalue);
      left = checkMultiplicativeExpression(left, right,"/");
      lvalue = false;

    } else if (lookahead == '%') {
      match('%');
      Type right = prefixExpression(lvalue);
      left = checkMultiplicativeExpression(left, right,"%");
      lvalue = false;

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

static Type additiveExpression(bool &lvalue)
{
  Type left = multiplicativeExpression(lvalue);

  while (1) {
    if (lookahead == '+') {
      match('+');
      Type right = multiplicativeExpression(lvalue);
      left = checkAdditiveExpression(left,right,"+");
      lvalue = false;

    } else if (lookahead == '-') {
      match('-');
      Type right = multiplicativeExpression(lvalue);
      left = checkSubtractiveExpression(left,right,"-");
      lvalue = false;

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

static Type relationalExpression(bool &lvalue)
{
  Type left = additiveExpression(lvalue);

  while (1) {
    if (lookahead == '<') {
      match('<');
      Type right = additiveExpression(lvalue);
      left = checkRelational(left,right,"<");
      lvalue = false;
    } else if (lookahead == '>') {
      match('>');
      Type right = additiveExpression(lvalue);
      left = checkRelational(left,right,">");
      lvalue = false;
    } else if (lookahead == LEQ) {
      match(LEQ);
      Type right = additiveExpression(lvalue);
      left = checkRelational(left,right,"<=");
      lvalue = false;
    } else if (lookahead == GEQ) {
      match(GEQ);
      Type right = additiveExpression(lvalue);
      left = checkRelational(left,right,">=");
      lvalue = false;
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

static Type equalityExpression(bool &lvalue)
{
  Type left = relationalExpression(lvalue);

  while (1) {
    if (lookahead == EQL) {
      match(EQL);
      Type right = relationalExpression(lvalue);
      left = checkEquality(left,right, "==");
      lvalue = false;

    } else if (lookahead == NEQ) {
      match(NEQ);
      Type right = relationalExpression(lvalue);
      left = checkEquality(left,right, "!=");
      lvalue = false;

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

static Type logicalAndExpression(bool &lvalue)
{
  Type left = equalityExpression(lvalue);

  while (lookahead == AND) {
    match(AND);
    Type right = equalityExpression(lvalue);
    left = checkLogical(left,right,"&&");
    lvalue = false;
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

static Type expression(bool &lvalue)
{
  Type left = logicalAndExpression(lvalue);

  while (lookahead == OR) {
    match(OR);
    Type right = logicalAndExpression(lvalue);
    left = checkLogical(left,right,"||");
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

static void statements()
{
  while (lookahead != '}')
  statement();
}


/*
* Function:	Assignment
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
  bool rvalue = false;
  Type left = expression(lvalue);
  debugLog("Evaluated left");
  debugLog(to_string(lvalue));
  if(lookahead == '=') {
    match('=');
    Type right = expression(rvalue);
    debugLog("Evaluated right");
    checkAssignment(left, right, lvalue);
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
*		  break ;
*		  return expression ;
*		  while ( expression ) statement
*		  for ( assignment ; expression ; assignment ) statement
*		  if ( expression ) statement
*		  if ( expression ) statement else statement
*		  assignment ;
*/

static void statement()
{
  bool lvalue = false;
  Type expr;

  if (lookahead == '{') {
    match('{');
    openScope();
    declarations();
    statements();
    closeScope();
    match('}');

  } else if (lookahead == BREAK) {
    match(BREAK);
    checkBreak(loop_counter);
    match(';');

  } else if (lookahead == RETURN) {
    match(RETURN);
    expr = expression(lvalue);
    checkReturn(expr, returnType);
    match(';');

  } else if (lookahead == WHILE) {
    match(WHILE);
    match('(');
    expr = expression(lvalue);
    loop_counter++;
    checkTest(expr);
    match(')');
    statement();

  } else if (lookahead == FOR) {
    match(FOR);
    match('(');
    assignment();
    match(';');
    expr = expression(lvalue);
    loop_counter++;
    checkTest(expr);
    match(';');
    assignment();
    match(')');
    statement();
    loop_counter--;

  } else if (lookahead == IF) {
    match(IF);
    match('(');
    expr = expression(lvalue);
    checkTest(expr);
    match(')');
    statement();

    if (lookahead == ELSE) {
      match(ELSE);
      statement();
    }

  } else {
    assignment();
    match(';');
  }
}


/*
* Function:	parameter
*
* Description:	Parse a parameter, which in Simple C is always a scalar
*		variable with optional pointer declarators.
*
*		parameter:
*		  specifier pointers identifier
*/

static Type parameter()
{
  int typespec;
  unsigned indirection;
  string name;
  Type type;


  typespec = specifier();
  indirection = pointers();
  name = expect(ID);

  type = Type(typespec, indirection);
  declareVariable(name, type);
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
*		  void pointers identifier remaining-parameters
*		  char pointers identifier remaining-parameters
*		  int pointers identifier remaining-parameters
*
*		remaining-parameters:
*		  empty
*		  , ...
*		  , parameter remaining-parameters
*/

static Parameters *parameters()
{
  int typespec;
  unsigned indirection;
  Parameters *params;
  string name;
  Type type;


  openScope();
  params = new Parameters();
  params->_varargs = false;

  if (lookahead == VOID) {
    typespec = VOID;
    match(VOID);

    if (lookahead == ')')
    return params;

  } else
  typespec = specifier();

  indirection = pointers();
  name = expect(ID);

  type = Type(typespec, indirection);
  declareVariable(name, type);
  params->_types.push_back(type);

  while (lookahead == ',') {
    match(',');

    if (lookahead == ELLIPSIS) {
      params->_varargs = true;
      match(ELLIPSIS);
      break;
    }

    params->_types.push_back(parameter());
  }

  return params;
}


/*
* Function:	globalDeclarator
*
* Description:	Parse a declarator, which in Simple C is either a scalar
*		variable, an array, or a function, with optional pointer
*		declarators.
*
*		global-declarator:
*		  pointers identifier
*		  pointers identifier [ num ]
*		  pointers identifier ( parameters )
*/

static void globalDeclarator(int typespec)
{
  unsigned indirection;
  string name;


  indirection = pointers();
  name = expect(ID);

  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typespec, indirection, number()));
    match(']');

  } else if (lookahead == '(') {
    match('(');
    declareFunction(name, Type(typespec, indirection, parameters()));
    closeScope();
    match(')');

  } else
  declareVariable(name, Type(typespec, indirection));
}


/*
* Function:	remainingDeclarators
*
* Description:	Parse any remaining global declarators after the first.
*
* 		remaining-declarators
* 		  ;
* 		  , global-declarator remaining-declarators
*/

static void remainingDeclarators(int typespec)
{
  while (lookahead == ',') {
    match(',');
    globalDeclarator(typespec);
  }

  match(';');
}


/*
* Function:	topLevelDeclaration
*
* Description:	Parse a global declaration or function definition.
*
* 		global-or-function:
* 		  specifier pointers identifier remaining-decls
* 		  specifier pointers identifier [ num ] remaining-decls
* 		  specifier pointers identifier ( parameters ) remaining-decls
* 		  specifier pointers identifier ( parameters ) { ... }
*/

static void topLevelDeclaration()
{
  int typespec;
  unsigned indirection;
  Parameters *params;
  string name;


  typespec = specifier();
  indirection = pointers();
  name = expect(ID);

  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typespec, indirection, number()));
    match(']');
    remainingDeclarators(typespec);

  } else if (lookahead == '(') {
    match('(');
    params = parameters();
    match(')');

    if (lookahead == '{') {
      returnType = Type(typespec, indirection);
      defineFunction(name, Type(typespec, indirection, params));
      match('{');
      declarations();
      statements();
      closeScope();
      match('}');

    } else {
      closeScope();
      declareFunction(name, Type(typespec, indirection, params));
      remainingDeclarators(typespec);
    }

  } else {
    declareVariable(name, Type(typespec, indirection));
    remainingDeclarators(typespec);
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
  lookahead = yylex();

  while (lookahead != DONE) {
    //std::cout << "Moving to line: " << yylineno << std::endl;
    topLevelDeclaration();
  }
  closeScope();
  exit(EXIT_SUCCESS);
}
