%{
    #include <iostream>

    static void skip_line_comment();
    static void skip_multiple_line_comment();
%}

%option noyywrap
DIGIT   0|[1-9][0-9]*
HEX     (0x|0X){DIGIT}[a-fA-F]*
ID      [a-zA-Z_][a-zA-Z_0-9]*
IGNORED [ \t\n]+ 
CHAR    '([^'\\\n]|\\.)'
STRING  \"([^\\\"]|\\.)*\"

%%

{IGNORED} {
    std::cout << "";
}

"//" { 
    skip_line_comment();
}

"/*" {
    skip_multiple_line_comment();
}

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

"volatile" {
    std::cout << "keyword " << yytext << std::endl;
}

"while" {
    std::cout << "keyword " << yytext << std::endl;
}

{STRING} {
    std::cout << "string " << yytext << std::endl;
}

{CHAR}  {
    std::cout << "character " << yytext << std::endl;
}

{ID} {
    std::cout << "identifier " << yytext << std::endl;
}

{DIGIT} {
    std::cout << "decimal " << yytext << std::endl;
}

{HEX} {
    std::cout << "hexadecimal " << yytext << std::endl;
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

. {
    std::cerr << yytext << std::endl;
}

%%

static void string_find() {
    
}

static void skip_line_comment() {
    int read;

    while((read = yyinput()) != '\n' && read != EOF)
        ;
    if(read == EOF)
        unput(read);
}

static void skip_multiple_line_comment() {
    
    int read;

    while(1)
    {
        while ( (read = yyinput()) != '*' && read != EOF )
            ;    
        if ( read == '*' ) {

            while ( (read = yyinput()) == '*' )
                ;
            if ( read == '/' )
                break;    /* found the end */
        }

        if ( read == EOF ) {
            perror( "EOF in comment" );
            break;
        }
    }
      
}

int main() {
    yylex();
    return 0;
}
