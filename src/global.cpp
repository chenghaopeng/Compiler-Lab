#include <iostream>
#include "global.h"

vector<AstNode>* ptrNodes;
vector<Symbol*> symbolList;
vector<InterRepresentation*> irList;

void init (vector<AstNode>* _nodes) {
    ptrNodes = _nodes;
}

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

vector<int> getSons (int u) {
    vector<int> sons;
    int x = get(u).lc;
    if (x == -1) return sons;
    sons.push_back(x);
    for (x = get(x).rc; x != -1; x = get(x).rc) {
        sons.push_back(x);
    }
    return sons;
}

string randomString (int size) {
    string s = "";
    for (int i = 0; i < size; ++i) {
        int t = rand() % (10 + 26 + 26);
        if (t < 10) s += (char)('0' + t);
        else if (t < 10 + 26) s += (char)('a' + t - 10);
        else s += (char)('A' + t - 10 - 26);
    }
    return s;
}

Symbol* symbolGet (string name, SymbolKind kind) {
    for (Symbol* symbol : symbolList) {
        if (symbol->name == name && symbol->kind == kind) {
            return symbol;
        }
    }
    return nullptr;
}

bool symbolExist (string name, SymbolKind kind) {
    return !!symbolGet(name, kind);
}

bool symbolConflit (string name, SymbolKind kind) {
    for (Symbol* symbol : symbolList) {
        if (symbol->name == name) {
            switch (symbol->kind) {
                case VAR:
                    if (kind == VAR || kind == STRUCT) {
                        return true;
                    }
                    break;
                case FIELD:
                    if (kind == FIELD) {
                        return true;
                    }
                    break;
                case STRUCT:
                    if (kind == VAR || kind == STRUCT) {
                        return true;
                    }
                    break;
                case FUNC:
                    if (kind == FUNC) {
                        return true;
                    }
                    break;
            }
        }
    }
    return false;
}

void symbolInsert (Symbol* symbol) {
    if (symbol) symbolList.push_back(symbol);
}

bool typeEqual (Type* t1, Type* t2) {
    if (!t1 || !t2) return false;
    if (t1->kind != t2->kind) return false;
    switch (t1->kind) {
        case BASIC:
            return t1->basic == t2->basic;
            break;
        case ARRAY:
            return typeEqual(t1->array.type, t2->array.type);
            break;
        case STRUCTURE:
            Field* f1 = t1->structure.field;
            Field* f2 = t2->structure.field;
            if (f1 == f2) return true;
            if ((f1 && !f2) || (!f1 && f2)) return false;
            while (f1 && f2) {
                if (!typeEqual(f1->type, f2->type)) return false;
                f1 = f1->next;
                f2 = f2->next;
            }
            return !f1 && !f2;
            break;
    }
    return true;
}

bool typeIsBasic (Type* type) {
    if (!type) return false;
    return type->kind == BASIC;
}

bool typeIsBasic (Type* type, BasicType typeKind) {
    if (!type) return false;
    if (!type->kind == BASIC) return false;
    return type->basic == typeKind;
}

bool functionParameterTypeEqual (FunctionParameter* p1, FunctionParameter* p2) {
    if (p1 == p2) return true;
    if ((p1 && !p2) || (!p1 && p2)) return false;
    while (p1 && p2) {
        if (!typeEqual(p1->type, p2->type)) return false;
        p1 = p1->next;
        p2 = p2->next;
    }
    if ((p1 && !p2) || (!p1 && p2)) return false;
    return true;
}

void irInsert (InterRepresentation* ir) {
    irList.push_back(ir);
}

void irPrint () {
    for (InterRepresentation* ir : irList) {
        cout << ir << '\n';
    }
}
