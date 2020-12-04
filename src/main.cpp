#include "syntax.tab.h"
#include <cstdio>
#include <cstdlib>
#include "lex.yy.c"
using namespace std;

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
        if (flag) fprintf(stderr, buf);
    }
    return 0;
}