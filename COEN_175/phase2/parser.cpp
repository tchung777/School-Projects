# include <cctype>
# include <iostream>
# include <unistd.h>
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
  else{
    //cout << "Trying to match " << t << endl;
    //cout << "Lookahead was " << lookahead << endl;
    report("syntax error");
    sleep(1);
  }
}

static void debugLog(string msg) {
  ;
  //cout << msg << endl;
  //sleep(1);
}
//-------------------------Expressions
static void expr() {
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
    else {
      break;
    }
  }
}

static void exprCMP()
{
  exprADDSUB();
  while(1) {
    if(lookahead == LESS_THAN) {
        match(LESS_THAN);
        exprADDSUB();
        cout << "ltn" << endl;
    } else if(lookahead == GREATER_THAN) {
        match(GREATER_THAN);
        exprADDSUB();
        cout << "gtn" << endl;
    } else if(lookahead == LESS_THANEQ) {
        match(LESS_THANEQ);
        exprADDSUB();
        cout << "leq" << endl;
    } else if(lookahead == GREATER_THANEQ) {
        match(GREATER_THANEQ);
        exprADDSUB();
        cout << "geq" << endl;
    } else break;
  }
}

static void exprADDSUB() {
    exprMULT();
    while(1) {
        if(lookahead == PLUS) {
            match(PLUS);
            exprMULT();
            cout << "add" << endl;
        }
        else if(lookahead == MINUS) {
            match(MINUS);
            exprMULT();
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
        if(lookahead == MULT) {
            match(MULT);
            exprPREFIX();
            cout << "mul" << endl;
        } else if(lookahead == DIV) {
            match(DIV);
            exprPREFIX();
            cout << "div" << endl;
        }else if(lookahead == MOD) {
            match(MOD);
            exprPREFIX();
            cout << "rem" << endl;
        } else break;
    }
}

static void exprPREFIX() {
    if(lookahead == B_AND) {
        match(B_AND);
        exprPREFIX();
        cout << "addr" << endl;
    } else if(lookahead == MULT) {
        match(MULT);
        exprPREFIX();
        cout << "deref" << endl;
    } else if(lookahead == NOT) {
        match(NOT);
        exprPREFIX();
        cout << "not" << endl;
    } else if(lookahead == MINUS) {
        match(MINUS);
        exprPREFIX();
        cout << "neg" << endl;
    } else if(lookahead == SIZEOF) {
        match(SIZEOF);
        exprPREFIX();
        cout << "sizeof" << endl;
    } else exprPOSFIX();
}

// expression [ expression ]
static void exprPOSFIX() {
    exprPRIMARY();
    while(lookahead == LEFT_BRACKET) {
        match(LEFT_BRACKET);
        expr();
        match(RIGHT_BRACKET);
        cout << "index" << endl;
    }
}

static void exprPRIMARY() {
    //Match id()
    debugLog("We got here.");
    if(lookahead == ID) {
      match(ID);
      if(lookahead == LEFT_PARAN) {
        debugLog("Entering left paran");
        match(LEFT_PARAN);
        if(lookahead == RIGHT_PARAN) {
          match(RIGHT_PARAN);
        } else {
          expressionList();
          match(RIGHT_PARAN);
        }
        debugLog("id()");
      }
      else {
        debugLog("id");
      }
    }

    //match NUM
    else if(lookahead == NUM) {
      match(NUM);
      debugLog("num");
    }

    //match STRING
    else if(lookahead == STRING) {
      match(STRING);
      debugLog("string");
    }

    //match character
    else if(lookahead == CHARACTER) {
      match(CHARACTER);
      debugLog("character");
    }

    //match (expr)
    else if(lookahead == LEFT_PARAN) {
      match(LEFT_PARAN);
      expr();
      match(RIGHT_PARAN);
      debugLog("(expr)");
    }
}
static void expressionList() {
  debugLog("Entering expression list.");
  expr();
  if(lookahead == COMMA) {
    match(COMMA);
    expressionList();
  }
}

//-----------------------------------------------------------------------------------------------------
void translation_unit() {
  specifier();
  pointers();
  debugLog("About to match ID.");
  match(ID);
  if(lookahead == LEFT_BRACKET) {
    match(LEFT_BRACKET);
    match(NUM);
    match(RIGHT_BRACKET);
    rest_of_globals();
  }

  else if(lookahead == LEFT_PARAN) {
    match(LEFT_PARAN);
    parameters();
    match(RIGHT_PARAN);
    if(lookahead == LEFT_BRACE) {
      match(LEFT_BRACE); // {
      declarations();
      statements();
      match(RIGHT_BRACE); // }
    }
    else rest_of_globals();
  }

  else rest_of_globals();
}

void rest_of_globals() {
  if(lookahead == SEMICOLON)
    match(SEMICOLON);
  else if(lookahead == COMMA) {
    match(COMMA);
    global_declarator();
    rest_of_globals();
  }
}

void global_declaration() {
    specifier();
    global_declarator_list();
}

void global_declarator_list() {
  global_declarator();
  if(lookahead == COMMA)
    global_declarator_list();
}

void global_declarator() {
    pointers();
    match(ID);
    if(lookahead == LEFT_PARAN) {
      match(LEFT_PARAN);
      parameters();
      match(RIGHT_PARAN);
    }

    else if(lookahead == LEFT_BRACKET) {
      match(LEFT_BRACKET);
      match(NUM);
      match(RIGHT_BRACKET);
    }
}

void pointers() {
    debugLog("Matching pointers...");
    while(lookahead == MULT) {
        match(MULT);
    }
}

int specifier() {
  debugLog("Matching specifier...");
  if(lookahead == INT) {
    match(INT); //Match INT
    debugLog("INT");
    return 1;
  }
  else if(lookahead == CHAR) {
    match(CHAR); //Match CHAR
    debugLog("CHAR");
    return 1;
  }
  else if(lookahead == VOID) {
    match(VOID); //Match VOID
    debugLog("VOID");
    return 1;
  }
  return 0;
}


void parameters() {
  debugLog("Checking parameters..");
  if(lookahead == VOID) {
    match(VOID);
    if(lookahead == RIGHT_PARAN) {
      return;
    }
    pointers();
    match(ID);
    remaining_params();
  } else {
    specifier();
    pointers();
    match(ID);
    remaining_params();
  }
}

void remaining_params() {
    if(lookahead == COMMA) {
      match(COMMA);
      if(lookahead == ELLIPSIS)
        match(ELLIPSIS);
      else {
        parameters();
        remaining_params();
      }
    }
}

void declarations() {
  debugLog("I'm inside declarations.");
  while(specifier())
    declaration();
}

void declaration() {
  specifier();
  declarator();
  while(lookahead == COMMA) {
    match(COMMA);
    declarator();
  }
  match(SEMICOLON);
}

void declarator_list() {
  declarator();
  if(lookahead == COMMA) {
    match(COMMA);
    declarator_list();
  }
}

void declarator() {
    pointers();
    match(ID);
    if(lookahead == LEFT_BRACKET) {
      match(LEFT_BRACKET);
      match(NUM);
      match(RIGHT_BRACKET);
    }

}

void statements() {
  while(lookahead != RIGHT_BRACE)
    statement();
}

void statement() {
    debugLog("I'm inside statement.");
    debugLog(to_string(lookahead));
    if(lookahead == LEFT_BRACE) {
      debugLog("Staments -> BRACE");
      match(LEFT_BRACE);
      declarations();
      statements();
      match(RIGHT_BRACE);
    }

    else if(lookahead == BREAK) {
      debugLog("Entering a break statement.");
      match(BREAK);
      match(SEMICOLON);
    }

    else if(lookahead == RETURN) {
      debugLog("Entering return statement.");
      match(RETURN);
      expr();
      debugLog("Finished expressions.");
      match(SEMICOLON);
    }

    else if(lookahead == WHILE) {
      debugLog("Entering a while statement.");
      match(WHILE);
      match(LEFT_PARAN);
      expr();
      match(RIGHT_PARAN);
      statement();
    }

    else if(lookahead == FOR) {
      debugLog("Entering a for loop.");
      match(FOR);
      match(LEFT_PARAN);
      assignment();
      match(SEMICOLON);
      expr();
      match(SEMICOLON);
      assignment();
      match(RIGHT_PARAN);
      statement();
    }

    else if(lookahead == IF) {
      debugLog("Entering an if statement.");
      match(IF);
      match(LEFT_PARAN);
      expr();
      match(RIGHT_PARAN);
      statement();
      if(lookahead == ELSE) {
        debugLog("Entering else statement.");
        match(ELSE);
        statement();
      }
    }

    else {
      assignment();
      match(SEMICOLON);
    }
}

void assignment() {
  debugLog("Entering assignments..");
  expr();
  if(lookahead == ASSIGN) {
    debugLog("Entering ASSIGN");
    match(ASSIGN);
    expr();
  }
}

int main() {
  //Set the lookahead to yylex() to begin scanning.
  lookahead = yylex();

  //While we haven't reach EOF.
  while(lookahead != 0) {
    // Kick off the process with exprOR.
    translation_unit();
  }
}
