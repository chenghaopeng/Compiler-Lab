#include <string>
#include <vector>
#include <iostream>
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

int newAstNode (char* name, int num, ...) {
    // AstNode node;
    // node.name = string(name);
    // if (num) {
    //     int tmp = node.lc = args[0];
    //     node.lineno = nodes[tmp].lineno;
    //     for (int i = 1; i < num; ++i) {
    //         nodes[tmp].rc = args[i];
    //         tmp = nodes[tmp].rc;
    //     }
    // }
    // else {
    //     node.lineno = args[0];
    //     node.opt = string(yytext);
    // }
    // nodes.push_back(node);
    return nodes.size() - 1;
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        yyread(argv[1]);
        yyparse();
        
    }
    return 0;
}