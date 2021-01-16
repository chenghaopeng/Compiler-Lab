#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using namespace std;

#define debug_print if (DEBUG) { cout << __LINE__ << ": " << __FUNCTION__ << " (" << production << ")\n"; }
#define debug_flag if (DEBUG) cout << "FLAG: " << __LINE__ << " " << __FUNCTION__ << "\n"
#define preprocess(node) AstNode node = get(u); string production = getProduction(u); vector<int> sons = getSons(u);

#define irGetRelop(kind) (kind == LT ? "<" : kind == GT ? ">" : kind == LE ? "<=" : kind == GE ? ">=" : kind == EQ ? "==" : "!=")

struct AstNode {
    string name;
    int lineno;
    string opt;
    int lc, rc;
    AstNode () { name = opt = ""; lineno = lc = rc = -1; }
};

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

enum RepresentationKind {
    LABEL,
    FUNCTION,
    ASSIGN,
    ADD,
    SUB,
    DIV,
    MUL,
    GOTO,
    IF_GOTO,
    RETURN,
    DEC,
    ARG,
    CALL,
    PARAM,
    READ,
    WRITE
};

enum RelopKind {
    LT,
    GT,
    LE,
    GE,
    EQ,
    NEQ
};

enum OperandKind {
    VARIABLE,
    CONSTANT,
    REF,
    DEREF
};

enum VariableKind {
    T,
    V
};

struct Variable {
    VariableKind kind;
    int id;
};

struct Operand {
    OperandKind kind;
    union {
        Variable* var;
        int constVal;
        Operand* ref;
        Operand* deref;
    };
};

struct InterRepresentation {
    RepresentationKind kind;
    int labelId;
    string functionName;
    struct { Operand* left; Operand* right; } assign;
    struct { Operand* left; Operand* op1; Operand* op2; } binaryAssign;
    int gotoId;
    struct { Operand* left; Operand* right; RelopKind kind; int gotoId; } if_goto;
    Operand* returnVal;
    struct { Variable* var; int size; } dec;
    Operand* arg;
    struct { Variable* ret; string functionName; } call;
    Variable* param;
    Operand* rw;
};

Symbol* symbolGet (string, SymbolKind);
bool symbolExist (string, SymbolKind);
bool symbolConflit (string, SymbolKind);
void symbolInsert (Symbol*);
bool typeEqual (Type*, Type*);
bool typeIsBasic (Type*);
bool typeIsBasic (Type*, BasicType);
bool functionParameterTypeEqual (FunctionParameter*, FunctionParameter*);

AstNode get (int);
string getProduction (int);
vector<int> getSons (int);
string randomString (int);

void irInsert (InterRepresentation*);
void irPrint ();
string irGetOperand (Operand*);
string irGetVariable (Variable*);

void init(vector<AstNode>*);
void semantic(int);
void ir(int);

#endif
