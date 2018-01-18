/*
 * File:	lexer.h
 *
 * Description:	This file contains the public function and variable
 *		declarations for the lexical analyzer for Simple C.
 */

# ifndef LEXER_H
# define LEXER_H

extern char *yytext;
extern int yylineno, numerrors;

extern int yylex();
extern void report(const std::string &str, const std::string &arg = "");

enum {
	AUTO = 1,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	FLOAT,
	FOR,
	GOTO,
	IF,
	INT,
	LONG,
	REGISTER,
	RETURN,
	SHORT,
	SIGNED,
	SIZEOF,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,
	OR,
	AND,
	EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	GREATER_THAN,
	INCREMENT,
	DECREMENT,
	ARROW,
	ELLIPSIS,
	NUM,
	HEXADECIMAL,
	ID,
	STRING,
	CHARACTER,
    OPERATOR
};

# endif /* LEXER_H */
