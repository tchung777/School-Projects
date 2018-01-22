# include <cctype>
# include <iostream>
# include "tokens.h"
# include "lexer.h"

int lookahead;
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
  if (t == lookahead)
    lookahead = yylex();
  else error();
}

static void exprOR() {
  exprOR_prime();
}

static void exprOR_prime()
{
  exprAND();
  while(1) {
    if(la == OR) {
      match(OR);
      exprAND()
      cout << "or" << endl;
    } else break;
  }
}

static void exprAND()
{
  exprAND_prime();
}

static void exprAND_prime()
{
  exprEQ();
  while(1) {
    if(la == AND) {
      match(AND);
      exprEQ();
      cout << "and" << endl;
    } else break;
  }
}

static void exprEQ()
{
  exprCMP();
  while(1) {
    if(la == EQUAL) {
      match(EQUAL);
      exprCMP();
      cout << "eql" << endl;
    }
    else if(la == NOT_EQUAL) {
      match(NOT_EQUAL);
      exprCMP();
      cout << "neq" << endl;
    }
  }
}

stativ void exprCMP()
{
  
}

int main() {
  lookahead = yylex();
  exprOR();
  return 0;
}
