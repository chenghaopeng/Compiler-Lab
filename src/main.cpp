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

void print (int u, int dep) {
    if (u < 0) return;
    for (int i = 0; i < dep; ++i) cout << "  ";
    if (nodes[u].lineno != -1) {
        cout << nodes[u].name;
    }
    cout << '\n';
    print(nodes[u].lc, dep + 1);
    print(nodes[u].rc, dep);
}

int main (int argc, char* argv[]) {
    nodes.push_back(AstNode{"", -1, "", -1, -1});
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
        print(nodes.size() - 1, 0);
    }
    return 0;
}