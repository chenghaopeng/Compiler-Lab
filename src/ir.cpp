#include "global.h"
#include "ir.h"
#include <vector>
#include <iostream>
#include <map>
using namespace std;

const int DEBUG = 0;

void irProgram (int u) {
    preprocess(Program)
    debug_print
    if (production == "ExtDefList") {}
}

void irExtDefList (int u) {
    preprocess(ExtDefList)
    debug_print
    if (production == "") { /*  */ }
    else if (production == "ExtDef ExtDefList") {}
}

void irExtDef (int u) {
    preprocess(ExtDef)
    debug_print
    if (production == "Specifier ExtDecList SEMI") {}
    else if (production == "Specifier SEMI") {}
    else if (production == "Specifier FunDec CompSt") {}
}

void irVarDec (int u) {
    preprocess(VarDec)
    debug_print
    if (production == "ID") {}
    else if (production == "VarDec LB INT RB") {}
}

void irFunDec (int u) {
    preprocess(FunDec)
    debug_print
    if (production == "ID LP VarList RP") {}
    else if (production == "ID LP RP") {}
}

void irVarList (int u) {
    preprocess(VarList)
    debug_print
    if (production == "ParamDec") {}
    else if (production == "ParamDec COMMA VarList") {}
}

void irParamDec (int u) {
    preprocess(ParamDec)
    debug_print
    if (production == "Specifier VarDec") {}
}

void irCompSt (int u) {
    preprocess(CompSt)
    debug_print
    if (production == "LC DefList StmtList RC") {}
}

void irStmtList (int u) {
    preprocess(StmtList)
    debug_print
    if (production == "") { /* */ }
    else if (production == "Stmt StmtList") {}
}

void irStmt (int u) {
    preprocess(Stmt)
    debug_print
    if (production == "Exp SEMI") {}
    else if (production == "CompSt") {}
    else if (production == "RETURN Exp SEMI") {}
    else if (production == "IF LP Exp RP Stmt") {}
    else if (production == "IF LP Exp RP Stmt ELSE Stmt") {}
    else if (production == "WHILE LP Exp RP Stmt") {}
}

void irDefList (int u) {
    preprocess(DefList)
    debug_print
    if (production == "") { /* */ }
    else if (production == "Def DefList") {}
}

void irDef (int u) {
    preprocess(Def)
    debug_print
    if (production == "Specifier DecList SEMI") {}
}

void irDecList (int u) {
    preprocess(DecList)
    debug_print
    if (production == "Dec") {}
    else if (production == "Dec COMMA DecList") {}
}

void irDec (int u) {
    preprocess(Dec)
    debug_print
    if (production == "VarDec") {}
    else if (production == "VarDec ASSIGNOP Exp") {}
}

void irExp (int u) {
    preprocess(Exp)
    debug_print
    if (production == "Exp ASSIGNOP Exp") {}
    else if (production == "Exp AND Exp" || production == "Exp OR Exp" || production == "Exp RELOP Exp") {}
    else if (production == "Exp PLUS Exp" || production == "Exp MINUS Exp" || production == "Exp STAR Exp" || production == "Exp DIV Exp") {}
    else if (production == "LP Exp RP") {}
    else if (production == "MINUS Exp") {}
    else if (production == "NOT Exp") {}
    else if (production == "ID LP Args RP") {}
    else if (production == "ID LP RP") {}
    else if (production == "Exp LB Exp RB") {}
    else if (production == "Exp DOT ID") {}
    else if (production == "ID") {}
    else if (production == "INT") {}
    else if (production == "FLOAT") {}
}

void irArgs (int u) {
    preprocess(Args)
    debug_print
    if (production == "Exp COMMA Args") {}
    else if (production == "Exp") {}
}

void ir (int root) {
    irProgram(root);
}
