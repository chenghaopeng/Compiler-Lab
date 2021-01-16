#include "global.h"
#include "ir.h"
#include <vector>
#include <iostream>
#include <map>
using namespace std;

const int DEBUG = 0;

map<Symbol*, Variable*> symbol_to_variable;

int V_count = 0, T_count = 0;
Variable* newVar (VariableKind kind) {
    Variable* var = new Variable;
    var->kind = kind;
    if (kind == V) {
        var->id = V_count++;
    }
    else {
        var->id = T_count++;
    }
}

int label_count = 0;
int newLabel () {
    return label_count++;
}

Operand* newConstant (int val) {
    Operand* operand = new Operand;
    operand->kind = CONSTANT;
    operand->constVal = val;
    return operand;
}

void irInsertFunction (const char* functionName) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = FUNCTION;
    code->functionName = functionName;
    irInsert(code);
}

void irInsertParam (Symbol* symbol) {
    Variable* var = newVar(V);
    InterRepresentation* code = new InterRepresentation;
    code->kind = PARAM;
    code->param = var;
    irInsert(code);
    symbol_to_variable[symbol] = var;
}

void irInsertLabel (int labelId) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = LABEL;
    code->labelId = labelId;
    irInsert(code);
}

void irInsertGoto (int labelId) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = GOTO;
    code->gotoId = labelId;
    irInsert(code);
}

void irInsertIfGoto (Operand* op1, Operand* op2, RelopKind kind, int labelId) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = IF_GOTO;
    code->if_goto.kind = kind;
    code->if_goto.left = op1;
    code->if_goto.right = op2;
    code->if_goto.gotoId = labelId;
    irInsert(code);
}

void irInsertAssign (Operand* left, Operand* right) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = ASSIGN;
    code->assign.left = left;
    code->assign.right = right;
    irInsert(code);
}

void irInsertDec (Variable* var, int size) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = DEC;
    code->dec.var = var;
    code->dec.size = size;
    irInsert(code);
}

void irInsertBinaryAssign (Operand* left, Operand* op1, Operand* op2, string op) {
    InterRepresentation* code = new InterRepresentation;
    if (op == "PLUS") {
        code->kind = ADD;
    }
    if (op == "MINUS") {
        code->kind = SUB;
    }
    if (op == "STAR") {
        code->kind = MUL;
    }
    if (op == "DIV") {
        code->kind = DIV;
    }
    code->binaryAssign.left = left;
    code->binaryAssign.op1 = op1;
    code->binaryAssign.op2 = op2;
    irInsert(code);
}

void irInsertWrite (Operand* operand) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = WRITE;
    code->rw = operand;
    irInsert(code);
}

void irInsertRead (Operand* operand) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = READ;
    code->rw = operand;
    irInsert(code);
}

void irInsertCall (Variable* var, const char* functionName) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = CALL;
    code->call.functionName = functionName;
    code->call.ret = var;
    irInsert(code);
}

void irInsertArg (Operand* operand) {
    InterRepresentation* code = new InterRepresentation;
    code->kind = ARG;
    code->arg = operand;
    irInsert(code);
}

void translateProgram (int u) {
    preprocess(Program)
    debug_print
    if (production == "ExtDefList") {
        translateExtDefList(sons[0]);
    }
}

void translateExtDefList (int u) {
    preprocess(ExtDefList)
    debug_print
    if (production == "") { /*  */ }
    else if (production == "ExtDef ExtDefList") {
        translateExtDef(sons[0]);
        translateExtDefList(sons[1]);
    }
}

void translateExtDef (int u) {
    preprocess(ExtDef)
    debug_print
    if (production == "Specifier ExtDecList SEMI") {}
    else if (production == "Specifier SEMI") {}
    else if (production == "Specifier FunDec CompSt") {
        translateFunDec(sons[1]);
        translateCompSt(sons[2]);
    }
}

void translateFunDec (int u) {
    preprocess(FunDec)
    debug_print
    irInsertFunction(get(sons[0]).opt.c_str());
    if (production == "ID LP VarList RP") {
        translateVarList(sons[2]);
    }
    else if (production == "ID LP RP") {}
}

void translateVarList (int u) {
    preprocess(VarList)
    debug_print
    if (production == "ParamDec") {
        translateParamDec(sons[0]);
    }
    else if (production == "ParamDec COMMA VarList") {
        translateParamDec(sons[0]);
        translateVarList(sons[1]);
    }
}

void translateParamDec (int u) {
    preprocess(ParamDec)
    debug_print
    if (production == "Specifier VarDec") {
        irInsertParam(symbolGet(get(get(sons[1]).lc).opt.c_str(), VAR));
    }
}

void translateCompSt (int u) {
    preprocess(CompSt)
    debug_print
    if (production == "LC DefList StmtList RC") {
        translateDefList(sons[1]);
        translateStmtList(sons[2]);
    }
}

void translateStmtList (int u) {
    preprocess(StmtList)
    debug_print
    if (production == "") { /* */ }
    else if (production == "Stmt StmtList") {
        translateStmt(sons[0]);
        translateStmtList(sons[1]);
    }
}

void translateStmt (int u) {
    preprocess(Stmt)
    debug_print
    if (production == "Exp SEMI") {
        translateExp(sons[0]);
    }
    else if (production == "CompSt") {
        translateCompSt(sons[0]);
    }
    else if (production == "RETURN Exp SEMI") {
        Operand* returnVal = translateExp(sons[1]);
        InterRepresentation* code = new InterRepresentation;
        code->kind = RETURN;
        code->returnVal = returnVal;
        irInsert(code);
    }
    else if (production == "IF LP Exp RP Stmt") {
        int trueLabel = newLabel();
        int nextLabel = newLabel();
        translateCondExp(sons[2], trueLabel, nextLabel);
        irInsertLabel(trueLabel);
        translateStmt(sons[4]);
        irInsertLabel(nextLabel);
    }
    else if (production == "IF LP Exp RP Stmt ELSE Stmt") {
        int trueLabel = newLabel();
        int falseLabel = newLabel();
        int nextLabel = newLabel();
        translateCondExp(sons[2], trueLabel, falseLabel);
        irInsertLabel(trueLabel);
        translateStmt(sons[4]);
        irInsertGoto(nextLabel);
        irInsertLabel(falseLabel);
        translateStmt(sons[6]);
        irInsertLabel(nextLabel);
    }
    else if (production == "WHILE LP Exp RP Stmt") {
        int beginLabel = newLabel();
        int trueLabel = newLabel();
        int nextLabel = newLabel();
        irInsertLabel(beginLabel);
        translateCondExp(sons[2], trueLabel, nextLabel);
        irInsertLabel(trueLabel);
        translateStmt(sons[4]);
        irInsertGoto(beginLabel);
        irInsertLabel(nextLabel);
    }
}

void translateDefList (int u) {
    preprocess(DefList)
    debug_print
    if (production == "") { /* */ }
    else if (production == "Def DefList") {
        translateDef(sons[0]);
        translateDefList(sons[1]);
    }
}

void translateDef (int u) {
    preprocess(Def)
    debug_print
    if (production == "Specifier DecList SEMI") {
        translateDecList(sons[1]);
    }
}

void translateDecList (int u) {
    preprocess(DecList)
    debug_print
    if (production == "Dec") {
        translateDec(sons[0]);
    }
    else if (production == "Dec COMMA DecList") {
        translateDec(sons[0]);
        translateDecList(sons[2]);
    }
}

void translateDec (int u) {
    preprocess(Dec)
    debug_print
    if (production == "VarDec") {
        translateVarDec(sons[0]);
    }
    else if (production == "VarDec ASSIGNOP Exp") {
        Operand* left = translateVarDec(sons[0]);
        Operand* right = translateExp(sons[2]);
        irInsertAssign(left, right);
    }
}

Operand* translateVarDec (int u) {
    preprocess(VarDec)
    debug_print
    if (production == "ID") {
        Symbol* symbol = symbolGet(get(sons[0]).opt, VAR);
        Variable* var = newVar(V);
        symbol_to_variable[symbol] = var;
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = var;
        return operand;
    }
    else if (production == "VarDec LB INT RB") {
        Symbol* symbol = symbolGet(get(get(sons[0]).lc).opt, VAR);
        Variable* var = newVar(V);
        symbol_to_variable[symbol] = var;
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = var;
        irInsertDec(var, atoi(get(sons[2]).opt.c_str()) * 4);
        return operand;
    }
}

Operand* translateExp (int u) {
    preprocess(Exp)
    debug_print
    if (production == "Exp ASSIGNOP Exp") {
        Operand* left = translateExp(sons[0]);
        Operand* right = translateExp(sons[2]);
        irInsertAssign(left, right);
        return left;
    }
    else if (production == "Exp AND Exp" || production == "Exp OR Exp" || production == "Exp RELOP Exp" || production == "NOT Exp") {
        Variable* var = newVar(T);
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = var;
        int trueLabel = newLabel();
        int falseLabel = newLabel();
        irInsertAssign(operand, newConstant(0));
        translateCondExp(u, trueLabel, falseLabel);
        irInsertLabel(trueLabel);
        irInsertAssign(operand, newConstant(1));
        irInsertLabel(falseLabel);
        return operand;
    }
    else if (production == "Exp PLUS Exp" || production == "Exp MINUS Exp" || production == "Exp STAR Exp" || production == "Exp DIV Exp") {
        Variable* var = newVar(T);
        Operand* left = new Operand;
        left->kind = VARIABLE;
        left->var = var;
        irInsertBinaryAssign(left, translateExp(sons[0]), translateExp(sons[2]), get(sons[1]).name);
        return left;
    }
    else if (production == "LP Exp RP") {
        return translateExp(sons[1]);
    }
    else if (production == "MINUS Exp") {
        Variable* var = newVar(T);
        Operand* left = new Operand;
        left->kind = VARIABLE;
        left->var = var;
        irInsertBinaryAssign(left, newConstant(0), translateExp(sons[1]), get(sons[0]).name);
        return left;
    }
    else if (production == "ID LP Args RP") {
        if (get(sons[0]).opt == "write") {
            Operand* operand = translateExp(get(sons[2]).lc);
            irInsertWrite(operand);
            return newConstant(0);
        }
        translateArgs(sons[2]);
        Variable* var = newVar(T);
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = var;
        irInsertCall(var, get(sons[0]).opt.c_str());
        return operand;
    }
    else if (production == "ID LP RP") {
        if (get(sons[0]).opt == "read") {
            Variable* var = newVar(T);
            Operand* operand = new Operand;
            operand->kind = VARIABLE;
            operand->var = var;
            irInsertRead(operand);
            return operand;
        }
        Variable* var = newVar(T);
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = var;
        irInsertCall(var, get(sons[0]).opt.c_str());
        return operand;
    }
    else if (production == "Exp LB Exp RB") {
        Operand* array = translateExp(sons[0]);
        Operand* index = translateExp(sons[2]);
        Operand* array_ref = new Operand;
        array_ref->kind = REF;
        array_ref->ref = array;
        Variable* array_ref_var = newVar(T);
        Operand* array_addr = new Operand;
        array_addr->kind = VARIABLE;
        array_addr->var = array_ref_var;
        irInsertAssign(array_addr, array_ref);
        Variable* offset_var = newVar(T);
        Operand* offset = new Operand;
        offset->kind = VARIABLE;
        offset->var = offset_var;
        irInsertBinaryAssign(offset, index, newConstant(4), "STAR");
        irInsertBinaryAssign(array_addr, array_addr, offset, "PLUS");
        Operand* res = new Operand;
        res->kind = DEREF;
        res->deref = array_addr;
        return res;
    }
    else if (production == "Exp DOT ID") {}
    else if (production == "ID") {
        Symbol* symbol = symbolGet(get(sons[0]).opt, VAR);
        Operand* operand = new Operand;
        operand->kind = VARIABLE;
        operand->var = symbol_to_variable[symbol];
        return operand;
    }
    else if (production == "INT") {
        return newConstant(atoi(get(sons[0]).opt.c_str()));
    }
    else if (production == "FLOAT") {}
}

void translateCondExp (int u, int trueLabel, int falseLabel) {
    preprocess(Exp)
    debug_print
    if (production == "Exp ASSIGNOP Exp" || production == "MINUS Exp" || production == "Exp PLUS Exp" || production == "Exp MINUS Exp" || production == "Exp STAR Exp" || production == "Exp DIV Exp" || production == "ID LP Args RP" || production == "ID LP RP" || production == "Exp LB Exp RB" || production == "ID") {
        Operand* operand = translateExp(u);
        irInsertIfGoto(operand, newConstant(0), NEQ, trueLabel);
        irInsertGoto(falseLabel);
    }
    else if (production == "Exp AND Exp") {
        int label = newLabel();
        translateCondExp(sons[0], label, falseLabel);
        irInsertLabel(label);
        translateCondExp(sons[2], trueLabel, falseLabel);
    }
    else if (production == "Exp OR Exp") {
        int label = newLabel();
        translateCondExp(sons[0], trueLabel, label);
        irInsertLabel(label);
        translateCondExp(sons[2], trueLabel, falseLabel);
    }
    else if (production == "Exp RELOP Exp") {
        Operand* op1 = translateExp(sons[0]);
        Operand* op2 = translateExp(sons[2]);
        string relop = get(sons[1]).opt;
        RelopKind kind;
        if (relop == "<") kind = LT;
        if (relop == ">") kind = GT;
        if (relop == "<=") kind = LE;
        if (relop == ">=") kind = GE;
        if (relop == "==") kind = EQ;
        if (relop == "!=") kind = NEQ;
        irInsertIfGoto(op1, op2, kind, trueLabel);
        irInsertGoto(falseLabel);
    }
    else if (production == "NOT Exp") {
        translateCondExp(sons[1], falseLabel, trueLabel);
    }
    else if (production == "LP Exp RP") {
        translateCondExp(sons[1], trueLabel, falseLabel);
    }
    else if (production == "Exp DOT ID") {}
    else if (production == "INT") {
        irInsertGoto(atoi(get(sons[0]).opt.c_str()) ? trueLabel : falseLabel);
    }
    else if (production == "FLOAT") {}
}

void translateArgs (int u) {
    preprocess(Args)
    debug_print
    if (production == "Exp COMMA Args") {
        translateArgs(sons[2]);
    }
    else if (production == "Exp") {}
    Operand* operand = translateExp(sons[0]);
    irInsertArg(operand);
}

void ir (int root) {
    Symbol* read = new Symbol;
    read->kind = FUNC;
    read->name = "read";
    FunctionDeclaim* read_declaim = new FunctionDeclaim;
    Type* read_declaim_type = new Type;
    read_declaim_type->kind = BASIC;
    read_declaim_type->basic = INT;
    read_declaim->type = read_declaim_type;
    read_declaim->parameter = nullptr;
    read->declaim = read_declaim;
    symbolInsert(read);
    Symbol* write = new Symbol;
    write->kind = FUNC;
    write->name = "write";
    FunctionDeclaim* write_declaim = new FunctionDeclaim;
    Type* write_declaim_type = new Type;
    write_declaim_type->kind = BASIC;
    write_declaim_type->basic = INT;
    write_declaim->type = write_declaim_type;
    write_declaim->parameter = new FunctionParameter;
    Type* write_declaim_param_type = new Type;
    write_declaim_param_type->kind = BASIC;
    write_declaim_param_type->basic = INT;
    write_declaim->parameter->type = write_declaim_param_type;
    write_declaim->parameter->next = nullptr;
    write->declaim = write_declaim;
    symbolInsert(write);
    translateProgram(root);
}
