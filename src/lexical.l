%{
    #include "syntax.tab.h"
    #include "stdio.h"
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
WS [ \t\n\r]+

INTZ 0
INTO 0[0-7]+
INTD [1-9]{DIGIT}+
INTH [(0x)(0X)][0-9a-fA-F]+
INT {INTZ}|{INTO}|{INTD}|{INTH}

FLOATN {DIGIT}+\.{DIGIT}+
FLOATE {DIGIT}*\.{DIGIT}*[eE][\+-]{0,1}{DIGIT}+
FLOAT {FLOATN}|{FLOATE}

COMMENTLINE //(.*?)\n
COMMENTLINES /\*(.*?)\*/
COMMENT {COMMENTLINE}|{COMMENTLINES}

SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS -
STAR \*
DIV /
AND &&
OR ||
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
ID ^{LETTER}({LETTER}|{DIGIT})*

%%

{INT} { printf("INT %s at Line %d.\n", yytext, yylineno); }

%%
