#include <cstdio>
#include <cstdlib>
using namespace std;

extern "C" {
    FILE* yyin;
    int yylex(void);
    int flag;
    char* buf;
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
        if (flag) fprintf(stderr, buf);
    }
    return 0;
}