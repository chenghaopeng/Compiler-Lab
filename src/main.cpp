#include <string>
#include <stack>
using namespace std;

extern "C" {
    void yyread(char*);
    int yyparse(void);
}

stack<string> ss;

extern "C" {
    void push_str(char *s) {
        ss.push(string(s));
    }
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
    }
    return 0;
}