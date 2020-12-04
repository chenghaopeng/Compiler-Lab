#include <cstdio>
#include <cstdlib>
using namespace std;

extern FILE* yyin;
extern void yylex();
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