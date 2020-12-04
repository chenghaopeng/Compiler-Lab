%{
    #include "stdio.h"

    #define _write len += sprintf(buf + len, s, a, b)

    int flag = 1;
    char buf[10000000];
    int len = 0;

    void add_printi (char* s, unsigned int a, int b) {
        _write;
    }

    void add_printf (char* s, double a, int b) {
        _write;
    }

    void add_print (char* s, char* a, int b) {
        _write;
    }

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
        add_printi("INT %u at Line %d.\n", n, lineno);
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
        add_printf("FLOAT %f at Line %d.\n", f, lineno);
    }

    void print_result(void) {
        if (flag) fprintf(stderr, buf);
    }

    void read_file(char *fn) {
        yyin = fopen(fn, "r");
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
    }
}

{FLOATN} { printf_float(0, yytext, yyleng, yylineno); }
{FLOATE} { printf_float(1, yytext, yyleng, yylineno); }
{INTZ} { printf_int(10, yytext, yyleng, yylineno); }
{INTO} { printf_int(8, yytext, yyleng, yylineno); }
{INTD} { printf_int(10, yytext, yyleng, yylineno); }
{INTH} { printf_int(16, yytext, yyleng, yylineno); }
{SEMI} { add_print("SEMI %s at Line %d.\n", yytext, yylineno); }
{COMMA} { add_print("COMMA %s at Line %d.\n", yytext, yylineno); }
{RELOP} { add_print("RELOP %s at Line %d.\n", yytext, yylineno); }
{ASSIGNOP} { add_print("ASSIGNOP %s at Line %d.\n", yytext, yylineno); }
{PLUS} { add_print("PLUS %s at Line %d.\n", yytext, yylineno); }
{MINUS} { add_print("MINUS %s at Line %d.\n", yytext, yylineno); }
{STAR} { add_print("STAR %s at Line %d.\n", yytext, yylineno); }
{DIV} { add_print("DIV %s at Line %d.\n", yytext, yylineno); }
{AND} { add_print("AND %s at Line %d.\n", yytext, yylineno); }
{OR} { add_print("OR %s at Line %d.\n", yytext, yylineno); }
{DOT} { add_print("DOT %s at Line %d.\n", yytext, yylineno); }
{NOT} { add_print("NOT %s at Line %d.\n", yytext, yylineno); }
{TYPE} { add_print("TYPE %s at Line %d.\n", yytext, yylineno); }
{LP} { add_print("LP %s at Line %d.\n", yytext, yylineno); }
{RP} { add_print("RP %s at Line %d.\n", yytext, yylineno); }
{LB} { add_print("LB %s at Line %d.\n", yytext, yylineno); }
{RB} { add_print("RB %s at Line %d.\n", yytext, yylineno); }
{LC} { add_print("LC %s at Line %d.\n", yytext, yylineno); }
{RC} { add_print("RC %s at Line %d.\n", yytext, yylineno); }
{STRUCT} { add_print("STRUCT %s at Line %d.\n", yytext, yylineno); }
{RETURN} { add_print("RETURN %s at Line %d.\n", yytext, yylineno); }
{IF} { add_print("IF %s at Line %d.\n", yytext, yylineno); }
{ELSE} { add_print("ELSE %s at Line %d.\n", yytext, yylineno); }
{WHILE} { add_print("WHILE %s at Line %d.\n", yytext, yylineno); }
{ID} { add_print("ID %s at Line %d.\n", yytext, yylineno); }
. { flag = 0; fprintf(stderr, "Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }

%%


