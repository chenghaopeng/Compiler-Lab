#include "global.h"
#include <vector>
#include <iostream>
using namespace std;

vector<AstNode>* ptrNodes;

AstNode get (int i) {
    return (*ptrNodes)[i];
}

string getProduction (int u) {
    int x = get(u).lc;
    if (x == -1) return "";
    string production = get(x).name;
    for (x = get(x).rc; x != -1; x = get(x).rc) {
        production += " " + get(x).name;
    }
    return production;
}

void analyseProgram (int u) {
    AstNode program = get(u);
    string production = getProduction(u);
    cout << production << '\n';
}

void semantic (vector<AstNode>* _nodes) {
    ptrNodes = _nodes;
    analyseProgram(ptrNodes->size() - 1);
}
