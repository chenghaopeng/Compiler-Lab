%{
    #include "syntax.tab.h"
    #include "stdio.h"
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
WS [ \t]+

INTZ 0
INTO 0[0-7]+
INTD [1-9]{DIGIT}*
INTH [(0x)(0X)][0-9a-fA-F]+
INT {INTZ}|{INTO}|{INTD}|{INTH}

FLOATN {DIGIT}+\.{DIGIT}+
FLOATE {DIGIT}*\.{DIGIT}*[eE][\+-]{0,1}{DIGIT}+
FLOAT {FLOATN}|{FLOATE}

COMMENTLINE \/\/(.*?)
COMMENTLINES \/\*[\s\S]*\*\/
COMMENT {COMMENTLINE}|{COMMENTLINES}

SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
ID {LETTER}({LETTER}|{DIGIT})*

%%

\n { yylineno++; }
{WS} {}
{COMMENT} {}

{INT} { printf("INT %s at Line %d.\n", yytext, yylineno); }
{FLOAT} { printf("FLOAT %s at Line %d.\n", yytext, yylineno); }
{SEMI} { printf("SEMI %s at Line %d.\n", yytext, yylineno); }
{COMMA} { printf("COMMA %s at Line %d.\n", yytext, yylineno); }
{ASSIGNOP} { printf("ASSIGNOP %s at Line %d.\n", yytext, yylineno); }
{RELOP} { printf("RELOP %s at Line %d.\n", yytext, yylineno); }
{PLUS} { printf("PLUS %s at Line %d.\n", yytext, yylineno); }
{MINUS} { printf("MINUS %s at Line %d.\n", yytext, yylineno); }
{STAR} { printf("STAR %s at Line %d.\n", yytext, yylineno); }
{DIV} { printf("DIV %s at Line %d.\n", yytext, yylineno); }
{AND} { printf("AND %s at Line %d.\n", yytext, yylineno); }
{OR} { printf("OR %s at Line %d.\n", yytext, yylineno); }
{DOT} { printf("DOT %s at Line %d.\n", yytext, yylineno); }
{NOT} { printf("NOT %s at Line %d.\n", yytext, yylineno); }
{TYPE} { printf("TYPE %s at Line %d.\n", yytext, yylineno); }
{LP} { printf("LP %s at Line %d.\n", yytext, yylineno); }
{RP} { printf("RP %s at Line %d.\n", yytext, yylineno); }
{LB} { printf("LB %s at Line %d.\n", yytext, yylineno); }
{RB} { printf("RB %s at Line %d.\n", yytext, yylineno); }
{LC} { printf("LC %s at Line %d.\n", yytext, yylineno); }
{RC} { printf("RC %s at Line %d.\n", yytext, yylineno); }
{STRUCT} { printf("STRUCT %s at Line %d.\n", yytext, yylineno); }
{RETURN} { printf("RETURN %s at Line %d.\n", yytext, yylineno); }
{IF} { printf("IF %s at Line %d.\n", yytext, yylineno); }
{ELSE} { printf("ELSE %s at Line %d.\n", yytext, yylineno); }
{WHILE} { printf("WHILE %s at Line %d.\n", yytext, yylineno); }
{ID} { printf("ID %s at Line %d.\n", yytext, yylineno); }
. { printf("Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }

%%

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
    }
    return 0;
}
