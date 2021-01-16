#include <string>
#include <vector>
using namespace std;

#define debug_print if (DEBUG) { cout << __LINE__ << ": " << __FUNCTION__ << " (" << production << ")\n"; }
#define debug_flag if (DEBUG) cout << "FLAG: " << __LINE__ << " " << __FUNCTION__ << "\n"
#define preprocess(node) AstNode node = get(u); string production = getProduction(u); vector<int> sons = getSons(u);

void analyseProgram (int);
void analyseExtDefList (int);
void analyseExtDef (int);
void analyseExtDecList (int, Type*);
Type* analyseSpecifier (int);
Type* analyseStructSpecifier (int);
string analyseOptTag (int);
string analyseTag (int);
Symbol* analyseVarDec (int, SymbolKind, Type*);
Symbol* analyseFunDec (int, Type*);
FunctionParameter* analyseVarList (int);
FunctionParameter* analyseParamDec (int);
void analyseCompSt (int, Symbol*);
void analyseStmtList (int, Symbol*);
void analyseStmt (int, Symbol*);
Field* analyseDefList (int, SymbolKind);
Field* analyseDef (int, SymbolKind);
Symbol* analyseDecList (int, SymbolKind, Type*);
Symbol* analyseDec (int, SymbolKind, Type*);
Type* analyseExp (int);
FunctionParameter* analyseArgs (int);
string analyseID (int);
int analyseINT (int);
