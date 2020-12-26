#include "global.h"
#include "semantic.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<AstNode>* ptrNodes;
vector<int> sons;

vector<Symbol*> symbolList;

Symbol* symbolGet (string name, SymbolKind kind) {
    for (Symbol* symbol : symbolList) {
        if (symbol->name == name || symbol->kind == kind) {
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
                    if (kind == VAR || kind == FIELD || kind == STRUCT) {
                        return true;
                    }
                    break;
                case FIELD:
                    if (kind == VAR || kind == FIELD) {
                        return true;
                    }
                    break;
                case STRUCT:
                    if (kind == VAR || kind == STRUCT) {
                        return true;
                    }
                    break;
                case FUNC:
                    if (kind == STRUCT) {
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
    if (t1 == t2) return true;
    if ((t1 && !t2) || (!t1 && t2)) return false;
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

void error (int code) {
    cout << "ERROR!! : " << code << '\n';
}

void semanticError (int code, int lineno, char* reason) {
    fprintf(stderr, "Error type %d at Line %d: %s.\n", code, lineno, reason);
}

AstNode get (int i) {
    return (*ptrNodes)[i];
}

string getProduction (int u) {
    sons.clear();
    int x = get(u).lc;
    if (x == -1) return "";
    string production = get(x).name;
    sons.push_back(x);
    for (x = get(x).rc; x != -1; x = get(x).rc) {
        production += " " + get(x).name;
        sons.push_back(x);
    }
    return production;
}

string randomString (int size) {
    srand(time(NULL));
    string s = "";
    for (int i = 0; i < size; ++i) {
        int t = rand() % (10 + 26 + 26);
        if (t < 10) s += (char)('0' + t);
        else if (t < 10 + 26) s += (char)('a' + t - 10);
        else s += (char)('A' + t - 10 - 26);
    }
    return s;
}

void analyseProgram (int u) {
    AstNode Program = get(u);
    string production = getProduction(u);
    if (production == "ExtDefList") {
        analyseExtDefList(sons[0]);
    }
    else error(1);
}

void analyseExtDefList (int u) {
    AstNode ExtDefList = get(u);
    string production = getProduction(u);
    if (production == "") { /*  */ }
    else if (production == "ExtDef ExtDefList") {
        analyseExtDef(sons[0]);
        analyseExtDefList(sons[1]);
    }
    else error(2);
}

void analyseExtDef (int u) {
    AstNode ExtDef = get(u);
    string production = getProduction(u);
    if (production == "Specifier ExtDecList SEMI") {
        Type* type = analyseSpecifier(sons[0]);
        type->valueType = LEFT;
        analyseExtDecList(sons[1], type);
    }
    else if (production == "Specifier SEMI") {
        Type* type = analyseSpecifier(sons[0]);
    }
    else if (production == "Specifier FunDec CompSt") {
        Type* type = analyseSpecifier(sons[0]);
        type->valueType = RIGHT;
        Symbol* funDec = analyseFunDec(sons[1], type);
        analyseCompSt(sons[2], funDec);
    }
    else error(3);
}

void analyseExtDecList (int u, Type* type) {
    AstNode ExtDecList = get(u);
    string production = getProduction(u);
    if (production == "VarDec") {
        analyseVarDec(sons[0], VAR, type);
    }
    else if (production == "VarDec COMMA ExtDecList") {
        analyseVarDec(sons[0], VAR, type);
        analyseExtDecList(sons[2], type);
    }
    else error(4);
}

Type* analyseSpecifier (int u) {
    AstNode Specifier = get(u);
    string production = getProduction(u);
    if (production == "TYPE") {
        Type* type = new Type;
        type->kind = BASIC;
        type->valueType = LEFT;
        AstNode TYPE = get(sons[0]);
        if (TYPE.name == "INT") type->basic = INT;
        else if (TYPE.name == "FLOAT") type->basic = FLOAT;
        else error(5);
        return type;
    }
    else if (production == "StructSpecifier") {
        return analyseStructSpecifier(sons[1]);
    }
    else error(5);
    return nullptr;
}

Type* analyseStructSpecifier (int u) {
    AstNode StructSpecifier = get(u);
    string production = getProduction(u);
    if (production == "STRUCT OptTag LC DefList RC") {
        string structName = analyseOptTag(sons[1]);
        Type* type = new Type;
        type->kind = STRUCTURE;
        type->valueType = LEFT;
        type->structure.name = structName;
        type->structure.field = analyseDefList(sons[3], FIELD);
        if (symbolConflit(structName, STRUCT)) {
            semanticError(16, StructSpecifier.lineno, "冲突的结构体名");
        }
        else {
            Symbol* symbol = new Symbol;
            symbol->name = structName;
            symbol->kind = STRUCT;
            symbol->type = type;
            symbolInsert(symbol);
        }
        return type;
    }
    else if (production == "STRUCT Tag") {
        string structName = analyseTag(sons[1]);
        if (symbolExist(structName, STRUCT)) {
            Symbol* symbol = symbolGet(structName, STRUCT);
            return symbol->type;
        }
        else {
            semanticError(17, StructSpecifier.lineno, "未定义的结构体");
            return nullptr;
        }
    }
    else error(6);
    return nullptr;
}

string analyseOptTag (int u) {
    AstNode OptTag = get(u);
    string production = getProduction(u);
    if (production == "") {
        return "0" + randomString(10);
    }
    else if (production == "ID") {
        return analyseID(sons[0]);
    }
    else error(7);
    return "";
}

string analyseTag (int u) {
    AstNode Tag = get(u);
    string production = getProduction(u);
    if (production == "ID") {
        return analyseID(sons[0]);
    }
    else error(8);
    return "";
}

Symbol* analyseVarDec (int u, SymbolKind kind, Type* type) {
    AstNode VarDec = get(u);
    string production = getProduction(u);
    if (production == "ID") {
        string id = analyseID(sons[0]);
        if (symbolConflit(id, kind)) {
            if (kind == VAR) semanticError(3, VarDec.lineno, "冲突的变量名");
            else if (kind == FIELD) semanticError(15, VarDec.lineno, "冲突的域名");
            else error(9);
            return nullptr;
        }
        else {
            Symbol* symbol = new Symbol;
            symbol->name = id;
            symbol->kind = kind;
            symbol->type = type;
            symbolInsert(symbol);
            return symbol;
        }
    }
    else if (production == "VarDec LB INT RB") {
        Symbol* symbol = analyseVarDec(sons[0], kind, type);
        if (symbol) {
            Type* type = new Type;
            type->kind = ARRAY;
            type->valueType = LEFT;
            type->array.type = symbol->type;
            type->array.size = analyseINT(sons[2]);
            symbol->type = type;
        }
        return symbol;
    }
    else error(9);
    return nullptr;
}

Symbol* analyseFunDec (int u, Type* type) {
    AstNode FunDec = get(u);
    string production = getProduction(u);
    if (production == "ID LP VarList RP") {
        string funcitonName = analyseID(sons[0]);
        Symbol* symbol = new Symbol;
        symbol->name = funcitonName;
        symbol->kind = FUNC;
        FunctionDeclaim* functionDeclaim = new FunctionDeclaim;
        functionDeclaim->type = type;
        functionDeclaim->parameter = analyseVarList(sons[2]);
        symbol->declaim = functionDeclaim;
        if (symbolConflit(funcitonName, FUNC)) {
            semanticError(4, FunDec.lineno, "冲突的函数名");
        }
        else {
            symbolInsert(symbol);
        }
        return symbol;
    }
    else if (production == "ID LP RP") {
        string funcitonName = analyseID(sons[0]);
        Symbol* symbol = new Symbol;
        symbol->name = funcitonName;
        symbol->kind = FUNC;
        FunctionDeclaim* functionDeclaim = new FunctionDeclaim;
        functionDeclaim->type = type;
        functionDeclaim->parameter = nullptr;
        symbol->declaim = functionDeclaim;
        if (symbolConflit(funcitonName, FUNC)) {
            semanticError(4, FunDec.lineno, "冲突的函数名");
        }
        else {
            symbolInsert(symbol);
        }
        return symbol;
    }
    else error(10);
    return nullptr;
}

FunctionParameter* analyseVarList (int u) {
    AstNode VarList = get(u);
    string production = getProduction(u);
    if (production == "ParamDec") {
        return analyseParamDec(sons[0]);
    }
    else if (production == "ParamDec COMMA VarList") {
        FunctionParameter* parameters = analyseParamDec(sons[0]);
        if (parameters) {
            parameters->next = analyseVarList(sons[2]);
        }
        else {
            parameters = analyseVarList(sons[2]);
        }
        return parameters;
    }
    else error(11);
    return nullptr;
}

FunctionParameter* analyseParamDec (int u) {
    AstNode ParamDec = get(u);
    string production = getProduction(u);
    if (production == "Specifier VarDec") {
        Type* type = analyseSpecifier(sons[0]);
        Symbol* symbol = analyseVarDec(sons[1], VAR, type);
        FunctionParameter* parameter = new FunctionParameter;
        parameter->type = symbol->type;
        return parameter;
    }
    else error(12);
    return nullptr;
}

void analyseCompSt (int u, Symbol* funDec) {
    AstNode CompSt = get(u);
    string production = getProduction(u);
    if (production == "LC DefList StmtList RC") {
        analyseDefList(sons[1], VAR);
        analyseStmtList(sons[2], funDec);
    }
    else error(13);
}

void analyseStmtList (int u, Symbol* funDec) {
    AstNode StmtList = get(u);
    string production = getProduction(u);
    if (production == "") { /* */ }
    else if (production == "Stmt StmtList") {
        analyseStmt(sons[0], funDec);
        analyseStmtList(sons[1], funDec);
    }
    else error(14);
}

void analyseStmt (int u, Symbol* funDec) {
    AstNode Stmt = get(u);
    string production = getProduction(u);
    if (production == "Exp SEMI") {
        Type* type = analyseExp(sons[0]);
    }
    else if (production == "CompSt") {
        analyseCompSt(sons[0], funDec);
    }
    else if (production == "RETURN Exp SEMI") {
        Type* type = analyseExp(sons[1]);
        if (typeEqual(type, funDec->type)) {
            semanticError(8, Stmt.lineno, "返回值类型错误");
        }
    }
    else if (production == "IF LP Exp RP Stmt") {
        Type* type = analyseExp(sons[2]);
        if (!typeIsBasic(type, INT)) {
            semanticError(7, Stmt.lineno, "if 条件不是 int");
        }
        analyseStmt(sons[4], funDec);
    }
    else if (production == "IF LP Exp RP Stmt ELSE Stmt") {
        Type* type = analyseExp(sons[2]);
        if (!typeIsBasic(type, INT)) {
            semanticError(7, Stmt.lineno, "if 条件不是 int");
        }
        analyseStmt(sons[4], funDec);
        analyseStmt(sons[6], funDec);
    }
    else if (production == "WHILE LP Exp RP Stmt") {
        Type* type = analyseExp(sons[2]);
        if (!typeIsBasic(type, INT)) {
            semanticError(7, Stmt.lineno, "while 条件不是 int");
        }
        analyseStmt(sons[4], funDec);
    }
    else error(15);
}

Field* analyseDefList (int u, SymbolKind kind) {
    AstNode DefList = get(u);
    string production = getProduction(u);
    if (production == "") { /* */ }
    else if (production == "Def DefList") {
        if (kind == VAR) {
            analyseDef(sons[0], kind);
            analyseDefList(sons[1], kind);
            return nullptr;
        }
        else if (kind == FIELD) {
            Field* field = analyseDef(sons[0], kind);
            if (field) {
                Field* tmp = field;
                while (field->next) field = field->next;
                field->next = analyseDefList(sons[1], kind);
                return tmp;
            }
            else {
                return analyseDefList(sons[1], kind);
            }
        }
        else error(16);
    }
    else error(16);
    return nullptr;
}

Field* analyseDef (int u, SymbolKind kind) {
    AstNode Def = get(u);
    string production = getProduction(u);
    if (production == "Specifier DecList SEMI") {
        Type* type = analyseSpecifier(sons[0]);
        Symbol* symbol = analyseDecList(sons[1], kind, type);
        if (kind == VAR) {
            return nullptr;
        }
        else if (kind == FIELD) {
            Field* head = new Field;
            Field* tail = head;
            head->next = nullptr;
            while (symbol) {
                Field* p = new Field;
                p->name = symbol->name;
                p->type = symbol->type;
                p->next = nullptr;
                tail = tail->next = p;
                symbol = symbol->next;
            }
            return head->next;
        }
        else error(17);
    }
    else error(17);
    return nullptr;
}

Symbol* analyseDecList (int u, SymbolKind kind, Type* type) {
    AstNode DecList = get(u);
    string production = getProduction(u);
    if (production == "Dec") {
        return analyseDec(sons[0], kind, type);
    }
    else if (production == "Dec COMMA DecList") {
        Symbol* symbol = analyseDec(sons[0], kind, type);
        if (symbol) {
            symbol->next = analyseDecList(sons[2], kind, type);
        }
        else {
            symbol = analyseDecList(sons[2], kind, type);
        }
        return symbol;
    }
    else error(18);
    return nullptr;
}

Symbol* analyseDec (int u, SymbolKind kind, Type* type) {
    AstNode Dec = get(u);
    string production = getProduction(u);
    if (production == "VarDec") {
        return analyseVarDec(sons[0], kind, type);
    }
    else if (production == "VarDec ASSIGNOP Exp") {
        Symbol* symbol = analyseVarDec(sons[0], kind, type);
        Type* expType = analyseExp(sons[2]);
        if (kind == VAR) {
            if (type->valueType != LEFT) {
                semanticError(6, Dec.lineno, "变量定义时赋值号左边不为左值表达式");
            }
            if (!typeEqual(type, expType)) {
                semanticError(5, Dec.lineno, "变量定义时赋值号两边类型不一致");
            }
            return symbol;
        }
        else if (kind == FIELD) {
            semanticError(15, Dec.lineno, "在域上初始化");
            return symbol;
        }
        else error(19);
    }
    else error(19);
    return nullptr;
}

Type* analyseExp (int u) {
    AstNode Exp = get(u);
    string production = getProduction(u);
    if (production == "Exp ASSIGNOP Exp") {
        Type* type1 = analyseExp(sons[0]);
        Type* type2 = analyseExp(sons[2]);
        if (type1 && type1->valueType != LEFT) {
            semanticError(6, Exp.lineno, "表达式赋值号左边不为左值表达式");
        }
        if (!typeEqual(type1, type2)) {
            semanticError(5, Exp.lineno, "表达式赋值号两边类型不一致");
        }
        return type1 ? type1 : type2;
    }
    else if (production == "Exp AND Exp" || production == "Exp OR Exp" || production == "Exp RELOP Exp") {
        Type* type1 = analyseExp(sons[0]);
        Type* type2 = analyseExp(sons[2]);
        if (!(typeIsBasic(type1, INT) && typeIsBasic(type2, INT))) {
            semanticError(7, Exp.lineno, "and/or/relop 的运算数不全是 int");
        }
        Type* type = new Type;
        type->kind = BASIC;
        type->valueType = RIGHT;
        type->basic = INT;
        return type;
    }
    else if (production == "Exp PLUS Exp" || production == "Exp MINUS Exp" || production == "Exp STAR Exp" || production == "Exp DIV Exp") {
        Type* type1 = analyseExp(sons[0]);
        Type* type2 = analyseExp(sons[2]);
        if (!(typeIsBasic(type1) && typeIsBasic(type2))) {
            semanticError(7, Exp.lineno, "plus/minus/star/div 的运算数不全是基本类型");
        }
        else if (!typeEqual(type1, type2)) {
            semanticError(7, Exp.lineno, "plus/minus/star/div 的运算数类型不一致");
        }
        return type1 ? type1 : type2;
    }
    else if (production == "LP Exp RP") {
        return analyseExp(sons[1]);
    }
    else if (production == "MINUS Exp") {
        Type* type = analyseExp(sons[1]);
        if (!typeIsBasic(type)) {
            semanticError(7, Exp.lineno, "minus 的运算数不是基本类型");
        }
        return type;
    }
    else if (production == "NOT Exp") {
        if (!typeIsBasic(analyseExp(sons[1]), INT)) {
            semanticError(7, Exp.lineno, "not 的运算数不全是 int");
        }
        Type* type = new Type;
        type->kind = BASIC;
        type->valueType = RIGHT;
        type->basic = INT;
        return type;
    }
    else if (production == "ID LP Args RP") {
        string functionName = analyseID(sons[0]);
        FunctionParameter* parameters = analyseArgs(sons[2]);
        if (symbolExist(functionName, FUNC)) {
            Symbol* function = symbolGet(functionName, FUNC);
            if (!functionParameterTypeEqual(parameters, function->declaim->parameter)) {
                semanticError(9, Exp.lineno, "函数调用参数不匹配");
            }
            return function->declaim->type;
        }
        else if (symbolExist(functionName, VAR)) {
            semanticError(11, Exp.lineno, "对变量函数调用");
            return nullptr;
        }
        else {
            semanticError(2, Exp.lineno, "函数未定义");
            return nullptr;
        }
    }
    else if (production == "ID LP RP") {
        string functionName = analyseID(sons[0]);
        if (symbolExist(functionName, FUNC)) {
            Symbol* function = symbolGet(functionName, FUNC);
            if (function->declaim->parameter) {
                semanticError(9, Exp.lineno, "函数调用参数不匹配");
            }
            return function->declaim->type;
        }
        else if (symbolExist(functionName, VAR)) {
            semanticError(11, Exp.lineno, "对变量函数调用");
            return nullptr;
        }
        else {
            semanticError(2, Exp.lineno, "函数未定义");
            return nullptr;
        }
    }
    else if (production == "Exp LB Exp RB") {
        Type* varType = analyseExp(sons[0]);
        Type* indType = analyseExp(sons[2]);
        if (!varType || varType->kind != ARRAY) {
            semanticError(10, Exp.lineno, "对非数组使用下标");
            return nullptr;
        }
        if (!typeIsBasic(indType, INT)) {
            semanticError(12, Exp.lineno, "数组下标不为整数");
        }
        return varType->array.type;
    }
    else if (production == "Exp DOT ID") {
        Type* varType = analyseExp(sons[0]);
        string fieldName = analyseID(sons[2]);
        if (!varType || varType->kind != STRUCT) {
            semanticError(13, Exp.lineno, "对非结构体使用域");
            return nullptr;
        }
        for (Field* field = varType->structure.field; field; field = field->next) {
            if (field->name == fieldName) {
                return field->type;
            }
        }
        semanticError(16, Exp.lineno, "访问结构体中不存在的域");
        return nullptr;
    }
    else if (production == "ID") {
        string id = analyseID(sons[0]);
        if (symbolExist(id, VAR)) {
            Symbol* symbol = symbolGet(id, VAR);
            return symbol->type;
        }
        else {
            semanticError(16, Exp.lineno, "变量未定义");
            return nullptr;
        }
    }
    else if (production == "INT") {
        Type* type = new Type;
        type->kind = BASIC;
        type->valueType = RIGHT;
        type->basic = INT;
        return type;
    }
    else if (production == "FLOAT") {
        Type* type = new Type;
        type->kind = BASIC;
        type->valueType = RIGHT;
        type->basic = FLOAT;
        return type;
    }
    else error(20);
    return nullptr;
}

FunctionParameter* analyseArgs (int u) {
    AstNode Args = get(u);
    string production = getProduction(u);
    if (production == "Exp COMMA Args") {
        Type* type = analyseExp(sons[0]);
        FunctionParameter* parameter = new FunctionParameter;
        parameter->type = type;
        parameter->next = analyseArgs(sons[2]);
        return parameter;
    }
    else if (production == "Exp") {
        Type* type = analyseExp(sons[0]);
        FunctionParameter* parameter = new FunctionParameter;
        parameter->type = type;
        return parameter;
    }
    else error(21);
    return nullptr;
}

string analyseID (int u) {
    return get(u).opt;
}

int analyseINT (int u) {
    return atoi(get(u).opt.c_str());
}

void semantic (vector<AstNode>* _nodes) {
    ptrNodes = _nodes;
    analyseProgram(ptrNodes->size() - 1);
}