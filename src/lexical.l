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
        printf("INT %u at Line %d.\n", n, lineno);
    }

    void printf_float (int type, char* s, int len, int lineno) {
        double f = 0, a = 0, b = 0;
        int i, j = 0;
        for (i = 0; s[i] != '.'; ++i) a = a * 10 + s[i] - '0';
        for (i++; s[i] >= '0' && s[i] <= '9'; ++i, ++j) b = b * 10 + s[i] - '0';
        while (j--) b /= 10;
        f = a + b;
        if (type) {
            int n = atoi(s + i + 1);
            if (n > 0) for (int i = 0; i < n; ++i) f *= 10;
            if (n < 0) for (int i = 0; i < -n; ++i) f /= 10;
        }
        printf("FLOAT %f at Line %d.\n", f, lineno);
    }
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

{WS} { for (int i = 0; i < yyleng; ++i) if (yytext[i] == '\n') yylineno++; }
{COMMENT} {
    for (int i = 0; i < yyleng; ++i) {
        if (yytext[i] == '\n') yylineno++;
        //if ((i > 1 && yytext[i - 1] == '/' && yytext[i] == '*') || (i < yyleng - 2 && yytext[i - 1] == '*' && yytext[i] == '/')) fprintf(stderr, "Error type A at Line %d: Mysterious character \"%c%c\".\n", yylineno, yytext[i - 1], yytext[i]);
    }
}

{FLOATN} { printf_float(0, yytext, yyleng, yylineno); }
{FLOATE} { printf_float(1, yytext, yyleng, yylineno); }
{INTZ} { printf_int(10, yytext, yyleng, yylineno); }
{INTO} { printf_int(8, yytext, yyleng, yylineno); }
{INTD} { printf_int(10, yytext, yyleng, yylineno); }
{INTH} { printf_int(16, yytext, yyleng, yylineno); }
{SEMI} { printf("SEMI %s at Line %d.\n", yytext, yylineno); }
{COMMA} { printf("COMMA %s at Line %d.\n", yytext, yylineno); }
{RELOP} { printf("RELOP %s at Line %d.\n", yytext, yylineno); }
{ASSIGNOP} { printf("ASSIGNOP %s at Line %d.\n", yytext, yylineno); }
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
. { fprintf(stderr, "Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }

%%

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
    }
    return 0;
}
