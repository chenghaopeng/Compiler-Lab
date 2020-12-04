#include <cstdio>
using namespace std;

extern "C" {
    FILE* yyin;
    int yylex(void);
    void print_result(void);
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        yylex();
        print_result();
    }
    return 0;
}