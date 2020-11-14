%{
    #include "syntax.tab.h"
    #include "stdio.h"

    void printf_int (int base, char* s, int len, int lineno) {
        int start = 0;
        unsigned int n = 0;
        if (base == 8) start = 1;
        if (base == 16) start = 2;
        for (; start < len; ++start) {
            n *= base;
            if (s[start] >= 'a' && s[start] <= 'z') n += s[start] - 'a' + 10;
            if (s[start] >= 'A' && s[start] <= 'Z') n += s[start] - 'A' + 10;
            if (s[start] >= '0' && s[start] <= '9') n += s[start] - '0';
        }
        fprintf(stderr, "INT %u at Line %d.\n", n, lineno);
    }

    void printf_float (int type, char* s, int len, int lineno) {
        float f = 0, a = 0, b = 0;
        int i;
        for (i = 0; s[i] != '.'; ++i) a = a * 10 + s[i] - '0';
        for (i++; s[i] >= '0' && s[i] <= '9'; ++i) b = b * 10 + s[i] - '0';
        while (b >= 1) b /= 10;
        f = a + b;
        if (type) {
            int n = atoi(s + i + 1);
            if (n > 0) for (int i = 0; i < n; ++i) f *= 10;
            if (n < 0) for (int i = 0; i < -n; ++i) f /= 10;
        }
        fprintf(stderr, "FLOAT %f at Line %d.\n", f, lineno);
    }
%}

DIGIT [0-9]
LETTER [a-zA-Z_]
WS [ \t]+

INTZ 0
INTO 0[0-7]+
INTD [1-9]{DIGIT}*
INTH 0(x|X)[0-9a-fA-F]+
INT {INTZ}|{INTO}|{INTD}|{INTH}

FLOATN {DIGIT}+\.{DIGIT}+
FLOATE {DIGIT}*\.{DIGIT}*[eE][\+-]{0,1}{DIGIT}+
FLOAT {FLOATN}|{FLOATE}

COMMENTLINE \/\/.*
COMMENTLINES \/\*(^(\*\/))*\*\/
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
{COMMENT} { for (int i = 0; i < yyleng; ++i) { if (yytext[i] == '\n') yylineno++; if ((i > 1 && yytext[i - 1] == '/' && yytext[i] == '*') || (i < yyleng - 2 && yytext[i - 1] == '*' && yytext[i] == '/')) fprintf(stderr, "Error type A at Line %d: Mysterious character \"%c%c\".\n", yylineno, yytext[i - 1], yytext[i]); } }
\/\*|\*\/ { fprintf(stderr, "Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }

{INTZ} { printf_int(10, yytext, yyleng, yylineno); }
{INTO} { printf_int(8, yytext, yyleng, yylineno); }
{INTD} { printf_int(10, yytext, yyleng, yylineno); }
{INTH} { printf_int(16, yytext, yyleng, yylineno); }
{FLOATN} { printf_float(0, yytext, yyleng, yylineno); }
{FLOATE} { printf_float(1, yytext, yyleng, yylineno); }
{SEMI} { fprintf(stderr, "SEMI %s at Line %d.\n", yytext, yylineno); }
{COMMA} { fprintf(stderr, "COMMA %s at Line %d.\n", yytext, yylineno); }
{ASSIGNOP} { fprintf(stderr, "ASSIGNOP %s at Line %d.\n", yytext, yylineno); }
{RELOP} { fprintf(stderr, "RELOP %s at Line %d.\n", yytext, yylineno); }
{PLUS} { fprintf(stderr, "PLUS %s at Line %d.\n", yytext, yylineno); }
{MINUS} { fprintf(stderr, "MINUS %s at Line %d.\n", yytext, yylineno); }
{STAR} { fprintf(stderr, "STAR %s at Line %d.\n", yytext, yylineno); }
{DIV} { fprintf(stderr, "DIV %s at Line %d.\n", yytext, yylineno); }
{AND} { fprintf(stderr, "AND %s at Line %d.\n", yytext, yylineno); }
{OR} { fprintf(stderr, "OR %s at Line %d.\n", yytext, yylineno); }
{DOT} { fprintf(stderr, "DOT %s at Line %d.\n", yytext, yylineno); }
{NOT} { fprintf(stderr, "NOT %s at Line %d.\n", yytext, yylineno); }
{TYPE} { fprintf(stderr, "TYPE %s at Line %d.\n", yytext, yylineno); }
{LP} { fprintf(stderr, "LP %s at Line %d.\n", yytext, yylineno); }
{RP} { fprintf(stderr, "RP %s at Line %d.\n", yytext, yylineno); }
{LB} { fprintf(stderr, "LB %s at Line %d.\n", yytext, yylineno); }
{RB} { fprintf(stderr, "RB %s at Line %d.\n", yytext, yylineno); }
{LC} { fprintf(stderr, "LC %s at Line %d.\n", yytext, yylineno); }
{RC} { fprintf(stderr, "RC %s at Line %d.\n", yytext, yylineno); }
{STRUCT} { fprintf(stderr, "STRUCT %s at Line %d.\n", yytext, yylineno); }
{RETURN} { fprintf(stderr, "RETURN %s at Line %d.\n", yytext, yylineno); }
{IF} { fprintf(stderr, "IF %s at Line %d.\n", yytext, yylineno); }
{ELSE} { fprintf(stderr, "ELSE %s at Line %d.\n", yytext, yylineno); }
{WHILE} { fprintf(stderr, "WHILE %s at Line %d.\n", yytext, yylineno); }
{ID} { fprintf(stderr, "ID %s at Line %d.\n", yytext, yylineno); }
. { fprintf(stderr, "Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }

%%

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
    }
    return 0;
}
