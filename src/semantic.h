#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

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

static vector<AstNode> nodes;

void semantic();

#endif
