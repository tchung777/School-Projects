/*
* File:	parser.c
*
* Description:	This file contains the public and private function and
*		variable definitions for the recursive-descent parser for
*		Simple C.
*/

# include <cstdlib>
# include <iostream>
# include "tokens.h"
# include "lexer.h"
# include "checker.h"

using namespace std;

static int lookahead;
static void expression();
static void statement();


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
  if (lookahead != t) error();

  lookahead = yylex();
}

static string expect(int t)
{
  string buf = yytext;
  match(t);
  return buf;
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

int specifier()
{
  int typeSpec;
  if (isSpecifier(lookahead)) {
    typeSpec = lookahead;
    match(lookahead);
  }else {
    typeSpec = ERROR;
    error();
  }

  return typeSpec;
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

unsigned pointers()
{
  unsigned count = 0;
  while (lookahead == '*') {
    match('*');
    count++;
  }
  return count;
}

static unsigned expectNum() {
  int value = strtoul(expect(NUM).c_str(), NULL, 0);
  return value;
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

static void declarator(int typeSpec)
{
  unsigned indirection;
  string name;

  indirection = pointers();
  name = expect(ID);

  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typeSpec, indirection, expectNum()));
    match(']');
  } else {
    declareVariable(name, Type(typeSpec, indirection));
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
  int typeSpec = specifier();
  declarator(typeSpec);

  while (lookahead == ',') {
    match(',');
    declarator(typeSpec);
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

static void primaryExpression()
{
  Type type;
  string name;
  int num;

  if (lookahead == '(') {
    match('(');
    expression();
    match(')');

  } else if (lookahead == CHARACTER) {
    name = expect(CHARACTER); //match(CHARACTER);

  } else if (lookahead == STRING) {
    name = expect(STRING); //match(STRING);

  } else if (lookahead == NUM) {
    num = expectNum(); //match(NUM);

  } else if (lookahead == ID) {
    name = expect(ID); //match(ID);
    
    if (lookahead == '(') {
      match('(');
      Type type(INT);

      if (lookahead != ')') {
        expression();
        callVariableFunction(name, type);

        while (lookahead == ',') {
          match(',');
          expression();
        }
      }

      match(')');
    }
    callVariableFunction(name, type);
  } else
    error();
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

static void postfixExpression()
{
  primaryExpression();

  while (lookahead == '[') {
    match('[');
    expression();
    match(']');
    //cout << "index" << endl;
  }
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

static void prefixExpression()
{
  if (lookahead == '!') {
    match('!');
    prefixExpression();
    //cout << "not" << endl;

  } else if (lookahead == '-') {
    match('-');
    prefixExpression();
    //cout << "neg" << endl;

  } else if (lookahead == '*') {
    match('*');
    prefixExpression();
    //cout << "deref" << endl;

  } else if (lookahead == '&') {
    match('&');
    prefixExpression();
    //cout << "addr" << endl;

  } else if (lookahead == SIZEOF) {
    match(SIZEOF);
    prefixExpression();
    //cout << "sizeof" << endl;

  } else
  postfixExpression();
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

static void multiplicativeExpression()
{
  prefixExpression();

  while (1) {
    if (lookahead == '*') {
      match('*');
      prefixExpression();
      //cout << "mul" << endl;

    } else if (lookahead == '/') {
      match('/');
      prefixExpression();
      //cout << "div" << endl;

    } else if (lookahead == '%') {
      match('%');
      prefixExpression();
      //cout << "rem" << endl;

    } else
    break;
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

static void additiveExpression()
{
  multiplicativeExpression();

  while (1) {
    if (lookahead == '+') {
      match('+');
      multiplicativeExpression();
      //cout << "add" << endl;

    } else if (lookahead == '-') {
      match('-');
      multiplicativeExpression();
      //cout << "sub" << endl;

    } else
    break;
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

static void relationalExpression()
{
  additiveExpression();

  while (1) {
    if (lookahead == '<') {
      match('<');
      additiveExpression();
      //cout << "ltn" << endl;

    } else if (lookahead == '>') {
      match('>');
      additiveExpression();
      //cout << "gtn" << endl;

    } else if (lookahead == LEQ) {
      match(LEQ);
      additiveExpression();
      //cout << "leq" << endl;

    } else if (lookahead == GEQ) {
      match(GEQ);
      additiveExpression();
      //cout << "geq" << endl;

    } else
    break;
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

static void equalityExpression()
{
  relationalExpression();

  while (1) {
    if (lookahead == EQL) {
      match(EQL);
      relationalExpression();
      //cout << "eql" << endl;

    } else if (lookahead == NEQ) {
      match(NEQ);
      relationalExpression();
      //cout << "neq" << endl;

    } else
    break;
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

static void logicalAndExpression()
{
  equalityExpression();

  while (lookahead == AND) {
    match(AND);
    equalityExpression();
    //cout << "and" << endl;
  }
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

static void expression()
{
  logicalAndExpression();

  while (lookahead == OR) {
    match(OR);
    logicalAndExpression();
    //cout << "or" << endl;
  }
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
  expression();

  if (lookahead == '=') {
    match('=');
    expression();
  }
}


/*
* Function:	statement
*
* Description:	Parse a statement.  Note that Simple C has so few
*		statements that we handle them all in this one function.
*
*		statement:
*		  { expectNum statements }
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
  if (lookahead == '{') {
    match('{');
    openScope("open block scope");
    declarations();
    statements();
    closeScope("close block scope");
    match('}');

  } else if (lookahead == BREAK) {
    match(BREAK);
    match(';');

  } else if (lookahead == RETURN) {
    match(RETURN);
    expression();
    match(';');

  } else if (lookahead == WHILE) {
    match(WHILE);
    match('(');
    expression();
    match(')');
    statement();

  } else if (lookahead == FOR) {
    match(FOR);
    match('(');
    assignment();
    match(';');
    expression();
    match(';');
    assignment();
    match(')');
    statement();

  } else if (lookahead == IF) {
    match(IF);
    match('(');
    expression();
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
  int typeSpec = specifier();
  unsigned indirection = pointers();
  string name = expect(ID);

  Type t = Type(typeSpec, indirection);
  declareVariable(name, t);
  return t;
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

static Parameters* parameters()
{
  Parameters *params;
  unsigned indirection;
  string name;
  int typeSpec;
  Type type;

  openScope("Open parameters");
  params = new Parameters();

  if (lookahead == VOID) {
    typeSpec = VOID;
    match(VOID);

    if (lookahead == ')')
      return params;

  } else
  typeSpec = specifier();

  indirection = pointers();
  name = expect(ID);

  type = Type(typeSpec, indirection);
  declareVariable(name, type);
  params->push_back(type);

  while (lookahead == ',') {
    match(',');
    if (lookahead == ELLIPSIS) {
      match(ELLIPSIS);
      break;
    }
    params->push_back(parameter());
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

static void globalDeclarator(int typeSpec)
{
  unsigned indirection;
  string name;

  indirection = pointers();
  name = expect(ID);

  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typeSpec, indirection, expectNum()));
    match(']');

  } else if (lookahead == '(') {
    match('(');
    declareFunction(name, Type(typeSpec, indirection, parameters()));
    closeScope("close declarator scope");
    match(')');
  } else {
    declareVariable(name, Type(typeSpec, indirection));
  }
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

static void remainingDeclarators(int typeSpec)
{
  while (lookahead == ',') {
    match(',');
    globalDeclarator(typeSpec);
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
  int typeSpec = specifier();
  unsigned indirection = pointers();
  string name = expect(ID);
  Parameters* params;


  if (lookahead == '[') {
    match('[');
    declareVariable(name, Type(typeSpec, indirection, expectNum())); //Effectively calling match(NUM)
    match(']');
    remainingDeclarators(typeSpec);

  } else if (lookahead == '(') {
    match('(');
    params = parameters();
    match(')');

    if (lookahead == '{') {
      defineFunction(name, Type(typeSpec, indirection, params));
      match('{');
      declarations();
      statements();
      closeScope("closing toplevel");
      match('}');

    } else {
      closeScope("closing top level");
      declareFunction(name, Type(typeSpec, indirection, params));
      remainingDeclarators(typeSpec);
    }

  } else {
    declareVariable(name, Type(typeSpec, indirection));
    remainingDeclarators(typeSpec);
  }
}


/*
* Function:	main
*
* Description:	Analyze the standard input stream.
*/

int main()
{
  openScope("opening globalscope");
  lookahead = yylex();

  while (lookahead != DONE) topLevelDeclaration();

  closeScope("closing global scope");
  exit(EXIT_SUCCESS);
}