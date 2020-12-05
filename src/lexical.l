%{
    #include "syntax.tab.h"
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
WS [ \t\n\r]+

FLOATN {DIGIT}+\.{DIGIT}+
FLOATE {DIGIT}*\.{DIGIT}*[eE][\+-]{0,1}{DIGIT}+
FLOAT {FLOATN}|{FLOATE}

INTZ 0
INTO 0[0-7]+
INTD [1-9]{DIGIT}*
INTH 0(x|X)[0-9a-fA-F]+
INT {INTZ}|{INTO}|{INTD}|{INTH}

COMMENTLINE \/\/.*
COMMENTLINES \/\*(.|\n)*\*\/
COMMENT {COMMENTLINE}|{COMMENTLINES}

SEMI ;
COMMA ,
RELOP >|<|>=|<=|==|!=
ASSIGNOP =
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

\n { yycolumn = 1; }
{WS} { for (int i = 0; i < yyleng; ++i) if (yytext[i] == '\n') yylineno++; }
{COMMENT} {
    for (int i = 0; i < yyleng; ++i) {
        if (yytext[i] == '\n') yylineno++;
    }
}

{FLOAT} { return FLOAT; }
{INT} { return INT; }
{SEMI} { return SEMI; }
{COMMA} { return COMMA; }
{RELOP} { return RELOP; }
{ASSIGNOP} { return ASSIGNOP; }
{PLUS} { return PLUS; }
{MINUS} { return MINUS; }
{STAR} { return STAR; }
{DIV} { return DIV; }
{AND} { return AND; }
{OR} { return OR; }
{DOT} { return DOT; }
{NOT} { return NOT; }
{TYPE} { return TYPE; }
{LP} { return LP; }
{RP} { return RP; }
{LB} { return LB; }
{RB} { return RB; }
{LC} { return LC; }
{RC} { return RC; }
{STRUCT} { return STRUCT; }
{RETURN} { return RETURN; }
{IF} { return IF; }
{ELSE} { return ELSE; }
{WHILE} { return WHILE; }
{ID} { return ID; }
. { /* no thing happened! */ }

%%


