#include "syntax.tab.h"
#include <cstdio>
#include <cstdlib>
using namespace std;

extern void yylex();
extern FILE* yyin;
extern int flag;
extern char* buf;

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
        if (flag) fprintf(stderr, buf);
    }
    return 0;
}