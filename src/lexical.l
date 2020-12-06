%{
    #include "syntax.tab.h"
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
    extern int newAstNode(char*, int, ...);
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
{WS} { }
{COMMENT} { }

{FLOAT} {  yylval = newAstNode("FLOAT", 0, yylineno); return FLOAT; }
{INT} { yylval = newAstNode("INT", 0, yylineno); return INT; }
{SEMI} { yylval = newAstNode("SEMI", 0, yylineno); return SEMI; }
{COMMA} { yylval = newAstNode("COMMA", 0, yylineno); return COMMA; }
{RELOP} { yylval = newAstNode("RELOP", 0, yylineno); return RELOP; }
{ASSIGNOP} { yylval = newAstNode("ASSIGNOP", 0, yylineno); return ASSIGNOP; }
{PLUS} { yylval = newAstNode("PLUS", 0, yylineno); return PLUS; }
{MINUS} { yylval = newAstNode("MINUS", 0, yylineno); return MINUS; }
{STAR} { yylval = newAstNode("STAR", 0, yylineno); return STAR; }
{DIV} { yylval = newAstNode("DIV", 0, yylineno); return DIV; }
{AND} { yylval = newAstNode("AND", 0, yylineno); return AND; }
{OR} { yylval = newAstNode("OR", 0, yylineno); return OR; }
{DOT} { yylval = newAstNode("DOT", 0, yylineno); return DOT; }
{NOT} { yylval = newAstNode("NOT", 0, yylineno); return NOT; }
{TYPE} { yylval = newAstNode("TYPE", 0, yylineno); return TYPE; }
{LP} { yylval = newAstNode("LP", 0, yylineno); return LP; }
{RP} { yylval = newAstNode("RP", 0, yylineno); return RP; }
{LB} { yylval = newAstNode("LB", 0, yylineno); return LB; }
{RB} { yylval = newAstNode("RB", 0, yylineno); return RB; }
{LC} { yylval = newAstNode("LC", 0, yylineno); return LC; }
{RC} { yylval = newAstNode("RC", 0, yylineno); return RC; }
{STRUCT} { yylval = newAstNode("STRUCT", 0, yylineno); return STRUCT; }
{RETURN} { yylval = newAstNode("RETURN", 0, yylineno); return RETURN; }
{IF} { yylval = newAstNode("IF", 0, yylineno); return IF; }
{ELSE} { yylval = newAstNode("ELSE", 0, yylineno); return ELSE; }
{WHILE} { yylval = newAstNode("WHILE", 0, yylineno); return WHILE; }
{ID} { yylval = newAstNode("ID", 0, yylineno); return ID; }
. { /* no thing happened! */ }

%%


