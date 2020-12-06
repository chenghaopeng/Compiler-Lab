#include <string>
#include <vector>
#include <iostream>
#include <cstdarg>
using namespace std;

extern "C" {
    void yyread(char*);
    int yyparse(void);
    char* yytext;
}

struct AstNode {
    string name;
    int lineno;
    string opt;
    int lc, rc;
};

vector<AstNode> nodes;

extern "C"
int newAstNode (char* name, int num, ...) {
    va_list args;
    va_start(args, num);
    AstNode node;
    node.name = string(name);
    if (num) {
        int tmp = node.lc = va_arg(args, int);
        node.lineno = nodes[tmp].lineno;
        for (int i = 1; i < num; ++i) {
            nodes[tmp].rc = va_arg(args, int);
            tmp = nodes[tmp].rc;
        }
    }
    else {
        node.lineno = va_arg(args, int);
        node.opt = string(yytext);
    }
    nodes.push_back(node);
    return nodes.size() - 1;
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
        cout << nodes[nodes.size() - 1].name;
    }
    return 0;
}