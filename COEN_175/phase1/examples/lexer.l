%{
    #include <iostream>
%}

%option noyywrap
DIGIT   0|[1-9][0-9]*
HEX     (0x|0X)DIGIT[a-fA-F]
ID      [a-zA-Z_][a-zA-Z_0-9]*
IGNORED [ \t\n]+ 
STRING  

%%
"auto" {
    std::cout << "keyword " << yytext << std::endl;
}

"break" {
    std::cout << "keyword " << yytext << std::endl;
}

"case" {
    std::cout << "keyword " << yytext << std::endl;
}

"char" {
    std::cout << "keyword " << yytext << std::endl;
}

"const" { 
    std::cout << "keyword " << yytext << std::endl;
}

"continue" {
    std::cout << "keyword " << yytext << std::endl;
}

"default" {
    std::cout << "keyword " << yytext << std::endl;
}

"do" {
    std::cout << "keyword " << yytext << std::endl;
}

"double" {
    std::cout << "keyword " << yytext << std::endl;
}

"else" {
    std::cout << "keyword " << yytext << std::endl;
}

"enum" {
    std::cout << "keyword " << yytext << std::endl;
}

"extern" {
    std::cout << "keyword " << yytext << std::endl;
}

"float" {
    std::cout << "keyword " << yytext << std::endl;
}

"for" {
    std::cout << "keyword " << yytext << std::endl;
}

"goto" {
    std::cout << "keyword " << yytext << std::endl;
}

"if" {
    std::cout << "keyword " << yytext << std::endl;
}

"int" {
    std::cout << "keyword " << yytext << std::endl;
}

"long" {
    std::cout << "keyword " << yytext << std::endl;
}

"register" {
    std::cout << "keyword " << yytext << std::endl;
}

"return" {
    std::cout << "keyword " << yytext << std::endl;
}

"short" {
    std::cout << "keyword " << yytext << std::endl;
}

"signed" {
    std::cout << "keyword " << yytext << std::endl;
}

"sizeof" {
    std::cout << "keyword " << yytext << std::endl;
}

"static" {
    std::cout << "keyword " << yytext << std::endl;
}

"struct" {
    std::cout << "keyword " << yytext << std::endl;
}

"switch" { 
    std::cout << "keyword " << yytext << std::endl;
}

"typedef" {
    std::cout << "keyword " << yytext << std::endl;
}

"union" { 
    std::cout << "keyword " << yytext << std::endl;
}

"unsigned" {
    std::cout << "keyword " << yytext << std::endl;
}

"void" {
    std::cout << "keyword " << yytext << std::endl;
} 

"voltatile" {
    std::cout << "keyword " << yytext << std::endl;
}

"while" {
    std::cout << "keyword " << yytext << std::endl;
}

"(" {
    std::cout << "operator " << yytext << std::endl;
}

")" {
    std::cout << "operator " << yytext << std::endl;
}

"{" {
    std::cout << "operator " << yytext << std::endl;
}

"}" {
    std::cout << "operator " << yytext << std::endl;
}

"*" {
    std::cout << "operator " << yytext << std::endl;
}

"..." {
    std::cout << "operator " << yytext << std::endl;
}

"," {
    std::cout << "operator " << yytext << std::endl;
}

";" {
    std::cout << "operator " << yytext << std::endl;
}

"=" {
    std::cout << "operator " << yytext << std::endl;
}

"||" {
    std::cout << "operator " << yytext << std::endl;
}

"&&" {
    std::cout << "operator " << yytext << std::endl;
}

"|" {
    std::cout << "operator " << yytext << std::endl;
}

"==" {
    std::cout << "operator " << yytext << std::endl;
}

"!=" {
    std::cout << "operator " << yytext << std::endl;
}

"<" {
    std::cout << "operator " << yytext << std::endl;
}

">" {
    std::cout << "operator " << yytext << std::endl;
}

"<=" {
    std::cout << "operator " << yytext << std::endl;
}

">=" {
    std::cout << "operator " << yytext << std::endl;
}

"+" {
    std::cout << "operator " << yytext << std::endl;
}

"-" {
    std::cout << "operator " << yytext << std::endl;
}

"/" {
    std::cout << "operator " << yytext << std::endl;
}

"%" {
    std::cout << "operator " << yytext << std::endl;
}

"&" {
    std::cout << "operator " << yytext << std::endl;
}

"!" {
    std::cout << "operator " << yytext << std::endl;
}

"++" {
    std::cout << "operator " << yytext << std::endl;
}

"--" {
    std::cout << "operator " << yytext << std::endl;
}

"->" {
    std::cout << "operator " << yytext << std::endl;
}

"[" {
    std::cout << "operator " << yytext << std::endl;
}

"]" {
    std::cout << "operator " << yytext << std::endl;
}

":" {
    std::cout << "operator " << yytext << std::endl;
}

"." {
    std::cout << "operator " << yytext << std::endl;
}

{DIGIT} {
    std::cout << "decimal " << yytext << std::endl;
}

{HEX} {
    std::cout << "hexadecimal " << yytext << std::endl;
}

{ID} {
    std::cout << "identifier " << yytext << std::endl;
}

{STRING} {
    std::cout << "string " << yytext << std::endl;
}

{IGNORED} {
    std::cout << "";
}

%%

int main() {
    yylex();
    return 1;
}
