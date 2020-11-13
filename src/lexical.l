%{
    #include "syntax.tab.h"
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
WS [ \t\n\r]+

INT0 0
INT8 ^0[0-7]+
INT10 ^[1-9]{DIGIT}+
INT16 ^[(0x)(0X)][0-9a-fA-f]+
INT {INT0}|{INT8}|{INT10}|{INT16}

FLOATN {DIGIT}+\.{DIGIT}+
FLOATE {DIGIT}*\.{DIGIT}*[eE][\+-]{0,1}{DIGIT}+
FLOAT {FLOATN}|{FLOATE}

COMMENTLINE //(.*?)\n
COMMENTLINES /\*(.*?)\*/
COMMENT COMMENTLINE|COMMENTLINES

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
