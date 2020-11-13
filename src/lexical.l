%{
    #include "syntax.tab.h"
    #include "stdio.h"
    int lineno = 1;
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

COMMENTLINE \/\/(.*?)\n
COMMENTLINES \/\*(.*?)\*\/
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

\n { lineno++; }
{WS} { printf("空\n"); }
{COMMENT} {}

{INT} { printf("INT %s at Line %d.\n", yytext, lineno); }
{FLOAT} { printf("FLOAT %s at Line %d.\n", yytext, lineno); }
{SEMI} { printf("SEMI %s at Line %d.\n", yytext, lineno); }
{COMMA} { printf("COMMA %s at Line %d.\n", yytext, lineno); }
{ASSIGNOP} { printf("ASSIGNOP %s at Line %d.\n", yytext, lineno); }
{RELOP} { printf("RELOP %s at Line %d.\n", yytext, lineno); }
{PLUS} { printf("PLUS %s at Line %d.\n", yytext, lineno); }
{MINUS} { printf("MINUS %s at Line %d.\n", yytext, lineno); }
{STAR} { printf("STAR %s at Line %d.\n", yytext, lineno); }
{DIV} { printf("DIV %s at Line %d.\n", yytext, lineno); }
{AND} { printf("AND %s at Line %d.\n", yytext, lineno); }
{OR} { printf("OR %s at Line %d.\n", yytext, lineno); }
{DOT} { printf("DOT %s at Line %d.\n", yytext, lineno); }
{NOT} { printf("NOT %s at Line %d.\n", yytext, lineno); }
{TYPE} { printf("TYPE %s at Line %d.\n", yytext, lineno); }
{LP} { printf("LP %s at Line %d.\n", yytext, lineno); }
{RP} { printf("RP %s at Line %d.\n", yytext, lineno); }
{LB} { printf("LB %s at Line %d.\n", yytext, lineno); }
{RB} { printf("RB %s at Line %d.\n", yytext, lineno); }
{LC} { printf("LC %s at Line %d.\n", yytext, lineno); }
{RC} { printf("RC %s at Line %d.\n", yytext, lineno); }
{STRUCT} { printf("STRUCT %s at Line %d.\n", yytext, lineno); }
{RETURN} { printf("RETURN %s at Line %d.\n", yytext, lineno); }
{IF} { printf("IF %s at Line %d.\n", yytext, lineno); }
{ELSE} { printf("ELSE %s at Line %d.\n", yytext, lineno); }
{WHILE} { printf("WHILE %s at Line %d.\n", yytext, lineno); }
{ID} { printf("ID %s at Line %d.\n", yytext, lineno); }
. { printf("Error type A at Line %d: Mysterious character \"%s\".\n", lineno, yytext); }

%%

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
    }
    return 0;
}
