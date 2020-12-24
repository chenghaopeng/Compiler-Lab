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

vector<AstNode> nodes;
