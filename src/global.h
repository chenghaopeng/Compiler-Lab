#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using namespace std;

struct AstNode {
    string name;
    int lineno;
    string opt;
    int lc, rc;
    AstNode () { name = opt = ""; lineno = lc = rc = -1; }
};

void semantic(vector<AstNode>*);

void ir(vector<AstNode>*);

#endif
