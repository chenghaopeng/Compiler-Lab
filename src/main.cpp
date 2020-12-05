#include <string>
#include <stack>
#include "io.h"
using namespace std;

extern "C" {
    void yyread(char*);
    int yyparse(void);
}

stack<string> ss;

void push_str(char *s) {
    ss.push(string(s));
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
    }
    return 0;
}