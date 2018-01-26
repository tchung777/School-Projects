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
	LESS_THANEQ,
	GREATER_THANEQ,
	LESS_THAN,
	GREATER_THAN,
	PLUS,
	MINUS,
	PIPE,
	ASSIGN,
	MULT,
	DIV,
	MOD,
	B_AND,
	NOT,
	LEFT_PARAN,
	RIGHT_PARAN,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	SEMICOLON,
	COLON,
	DOT,
	COMMA,
	INCREMENT,
	DECREMENT,
	ARROW,
	ELLIPSIS,
	NUM,
	HEXADECIMAL,
	ID,
	STRING,
	CHARACTER,

};

# endif /* LEXER_H */
