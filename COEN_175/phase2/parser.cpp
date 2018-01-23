# include <cctype>
# include <iostream>
# include "parser.h"
# include "lexer.h"

using namespace std;
int lookahead;

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
  else report("syntax error");
}

static void exprOR() {
  exprAND();
  while(lookahead == OR) {
    match(OR);
    exprAND();
    cout << "or" << endl;
  }
}


static void exprAND()
{
  exprEQ();
  while(lookahead == AND) {
    match(AND);
    exprEQ();
    cout << "and" << endl;
  }
}

void exprEQ()
{
  exprCMP();
  while(1) {
    if(lookahead == EQUAL) {
      match(EQUAL);
      exprCMP();
      cout << "eql" << endl;
    }
    else if(lookahead == NOT_EQUAL) {
      match(NOT_EQUAL);
      exprCMP();
      cout << "neq" << endl;
    }
  }
}

static void exprCMP()
{
  exprADDSUB();
  while(1) {
    if(lookahead == '<') {
        match('<');
        exprADDSUB();
        cout << "ltn" << endl;
    } else if(lookahead == '>') {
        match('>');
        exprADDSUB();
        cout << "gtn" << endl;
    } else if(lookahead == LESS_THAN) {
        match(LESS_THAN);
        exprADDSUB();
        cout << "leq" << endl;
    } else if(lookahead == GREATER_THAN) {
        match(GREATER_THAN);
        exprADDSUB();
        cout << "geq" << endl;
    } else break;
  }  
}

static void exprADDSUB() {
    exprMULT();
    while(1) {
        if(lookahead == '+') {
            match('+');
            exprMULT();
            cout << "add" << endl;
        }
        else if(lookahead == '-') {
            match('-');
            exprPREFIX();
            cout << "sub" << endl;
        }
        else {
            break;
        }
    }
}

static void exprMULT() {
    exprPREFIX();
    while(1) {
        if(lookahead == '*') {
            match('*');
            exprPREFIX();
            cout << "mul" << endl;
        } else if(lookahead == '/') {
            match('/');
            exprPREFIX();
            cout << "div" << endl;
        }else if(lookahead == '%') {
            match('%');
            exprPREFIX();
            cout << "rem" << endl;
        } else break;
    }
}

static void exprPREFIX() {
    exprPOSFIX();
    while(1) {
        if(lookahead == '&') {
            match('&');
            exprPOSFIX();
            cout << "addr" << endl;
        } else if(lookahead == '*') {
            match('*');
            exprPOSFIX();
            cout << "deref" << endl;
        } else if(lookahead == '!') {
            match('!');
            exprPOSFIX();
            cout << "not" << endl;
        } else if(lookahead == '-') {
            match('-');
            exprPOSFIX();
            cout << "neg" << endl;
        } else if(lookahead == SIZEOF) {
            match(SIZEOF);
            exprPOSFIX();
            cout << "sizeof" << endl;
        } else break;
   }
}

static void exprPOSFIX() {
    exprPRIMARY();
    while(lookahead == '[') {    
        match('[');
        exprPRIMARY();
        match(']');
        cout << "index" << endl;
    }
}

static void exprPRIMARY() {
    cout << lookahead << endl;
}

//-----------------------------------------------------------------------------------------------------

void ptrs() {
    while(lookahead == '*') {
        match('*');
    }
}

void declarator() {
    ptrs();
    match(ID);
    if(lookahead == '[') { 
        match('[');
        match(NUM);
        match(']');
    } else {
        ; //Do nothing.
    }
}

int main() {
  //Set the lookahead to yylex() to begin scanning.
  lookahead = yylex();

  //While we haven't reach EOF.
  while(lookahead != 0) {
    // Kick off the process with exprOR.
    exprOR();
  }
}
