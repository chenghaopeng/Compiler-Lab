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

void init(vector<AstNode>*);
void semantic(int);
void ir(int);

#endif
