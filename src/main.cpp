#include <string>
#include <vector>
#include <iostream>
#include <cstdarg>
#include <sstream>
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
    AstNode () { name = opt = ""; lineno = lc = rc = -1; }
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
    if (u < 0 || nodes[u].lineno == -1) return;
    for (int i = 0; i < dep; ++i) cout << "  ";
    cout << nodes[u].name;
    if (nodes[u].name == "ID" || nodes[u].name == "TYPE" || nodes[u].name == "INTEGER" || nodes[u].name == "FLOAT") cout << ": " << nodes[u].opt;
    if (nodes[u].name[1] >= 'a' && nodes[u].name[1] <= 'z') cout << " (" << nodes[u].lineno << ")";
    cout << "\n";
    print(nodes[u].lc, dep + 1);
    print(nodes[u].rc, dep);
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
        print(nodes.size() - 1, 0);
    }
    return 0;
}