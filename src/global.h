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

vector<AstNode>* ptrNodes;

AstNode get (int);
string getProduction (int);
vector<int> getSons (int);

void init(vector<AstNode>*);
void semantic();
void ir();

#endif
