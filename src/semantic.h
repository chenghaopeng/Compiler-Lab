#include <string>
#include <vector>
using namespace std;

#define debug_print if (DEBUG) { cout << __LINE__ << ": " << __FUNCTION__ << " (" << production << ")\n"; }
#define debug_flag if (DEBUG) cout << "FLAG: " << __LINE__ << " " << __FUNCTION__ << "\n"
#define preprocess(node) AstNode node = get(u); string production = getProduction(u); vector<int> sons = getSons(u);

struct Type;

enum TypeKind {
    BASIC,
    ARRAY,
    STRUCTURE
};

enum ValueType {
    LEFT,
    RIGHT
};

enum BasicType {
    INT,
    FLOAT
};

struct ArrayType {
    Type* type;
    int size;
};

struct Field {
    Type* type;
    string name;
    Field* next;
    Field () {
        next = nullptr;
    }
};

struct StructureType {
    Field* field;
};

struct Type {
    TypeKind kind;
    ValueType valueType;
    union {
        BasicType basic;
        ArrayType array;
        StructureType structure;
    };
    Type () {}
};

enum SymbolKind {
    VAR,
    FIELD,
    STRUCT,
    FUNC
};

struct FunctionParameter {
    Type* type;
    FunctionParameter* next;
    FunctionParameter () {
        next = nullptr;
    }
};

struct FunctionDeclaim {
    Type* type;
    FunctionParameter* parameter;
};

struct Symbol {
    SymbolKind kind;
    string name;
    union {
        Type* type;
        FunctionDeclaim* declaim;
    };
    Symbol* next;
    Symbol () {
        next = nullptr;
    }
};

vector<Symbol*> symbolList;

Symbol* symbolGet (string, SymbolKind);
bool symbolExist (string, SymbolKind);
bool symbolConflit (string, SymbolKind);
void symbolInsert (Symbol*);
bool typeEqual (Type*, Type*);
bool typeIsBasic (Type*);
bool typeIsBasic (Type*, BasicType);
bool functionParameterTypeEqual (FunctionParameter*, FunctionParameter*);

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
