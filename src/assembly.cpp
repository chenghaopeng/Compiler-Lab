#include "global.h"
#include <map>
#include <iostream>
#include <set>

#define oadd outs.push_back
#define oinsert(a) outs.insert(outs.begin() + 3, a)

vector<string> outs;
map<string, bool> inData;
set<string> vars;
string curFunction;

int regc = 0;

string newReg () {
    return "$" + to_string(regc++);
}

string mksur (string var, string reg) {
    vars.insert(var);
    if (!inData[var]) {
        inData[var] = true;
        oinsert(var + ": .word 0");
    }
    string tmp = newReg();
    oadd("la " + tmp + ", " + var);
    oadd("lw " + reg + ", 0(" + tmp + ")");
    return reg;
}

string mksur (Operand* operand) {
    string var, reg = newReg(), tmp;
    switch (operand->kind) {
    case VARIABLE:
        mksur(irGetVariable(operand->var), reg);
        break;
    case CONSTANT:
        oadd("li " + reg + ", " + to_string(operand->constVal));
        break;
    case REF:
        var = irGetVariable(operand->ref->var);
        oadd("la " + reg + ", " + var);
        break;
    case DEREF:
        var = irGetVariable(operand->deref->var);
        tmp = newReg();
        oadd("la " + tmp + ", " + var);
        oadd("lw " + reg + ", 0(" + tmp + ")");
        oadd("lw " + reg + ", 0(" + reg + ")");
        break;
    default:
        break;
    }
    return reg;
}

void save (Operand* operand, string reg) {
    string var, tmp;
    switch (operand->kind) {
    case VARIABLE:
        var = irGetVariable(operand->var);
        tmp = newReg();
        oadd("la " + tmp + ", " + var);
        oadd("sw " + reg + ", 0(" + tmp + ")");
        break;
    case CONSTANT:
        break;
    case REF:
        break;
    case DEREF:
        var = irGetVariable(operand->deref->var);
        tmp = newReg();
        oadd("la " + tmp + ", " + var);
        oadd("lw " + tmp + ", 0(" + tmp + ")");
        oadd("sw " + reg + ", 0(" + tmp + ")");
        break;
    default:
        break;
    }
}

void save (string var, string reg) {
    string tmp = newReg();
    oadd("la " + tmp + ", " + var);
    oadd("sw " + reg + ", 0(" + tmp + ")");
}

string getLabel (int id) {
    return "label" + to_string(id);
}

string getFunctionName (string name) {
    return name == "main" ? name : ("function_" + name);
}

void asmPrint (FILE* fp) {
    oadd(".data");
    oadd("_prompt: .asciiz \"Enter an integer:\"");
    oadd("_ret: .asciiz \"\\n\"");
    oadd("_function_stack: .space 524280");
    oadd(".globl main");
    oadd(".text");
    oadd("read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n\nwrite:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");
    string r1, r2, r3;
    vector<string> varstmp;
    for (int i = 0; i < irCount(); ++i) {
        InterRepresentation* ir = irGet(i);
        regc = 8;
        switch (ir->kind) {
        case LABEL:
            oadd(getLabel(ir->labelId) + ":");
            break;
        case FUNCTION:
            oadd(getFunctionName(ir->functionName) + ":");
            vars = set<string>();
            if (ir->functionName == "main") {
                oadd("la $3, _function_stack");
            }
            curFunction = ir->functionName;
            break;
        case ASSIGN:
            r1 = mksur(ir->assign.left);
            r2 = mksur(ir->assign.right);
            oadd("move " + r1 + ", " + r2);
            save(ir->assign.left, r1);
            break;
        case ADD:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            oadd("add " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case SUB:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            oadd("sub " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case DIV:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            oadd("div " + r2 + ", " + r3);
            oadd("mflo " + r1);
            save(ir->binaryAssign.left, r1);
            break;
        case MUL:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            oadd("mul " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case GOTO:
            oadd("j " + getLabel(ir->gotoId));
            break;
        case IF_GOTO:
            r1 = mksur(ir->if_goto.left);
            r2 = mksur(ir->if_goto.right);
            r3 = asmGetRelop(ir->if_goto.kind);
            oadd(r3 + " " + r1 + ", " + r2 + ", " + getLabel(ir->if_goto.gotoId));
            break;
        case RETURN:
            r1 = mksur(ir->returnVal);
            oadd("move $2, " + r1);
            oadd("jr $ra\n");
            break;
        case DEC:
            r1 = irGetVariable(ir->dec.var);
            inData[r1] = true;
            oinsert(r1 + ": .space " + to_string(ir->dec.size));
            break;
        case ARG:
            r1 = mksur(ir->arg);
            oadd("addi $sp, $sp, -4");
            oadd("sw " + r1 + ", 0($sp)");
            break;
        case CALL:
            varstmp = vector<string>();
            if (curFunction == ir->call.functionName) {
                r1 = newReg();
                for (set<string>::iterator it = vars.begin(); it != vars.end(); ++it) {
                    r1 = mksur(*it, r1);
                    oadd("sw " + r1 + ", 0($3)");
                    oadd("addi $3, $3, 4");
                    regc--;
                    varstmp.push_back(*it);
                }
                regc--;
            }
            oadd("addi $sp, $sp, -4");
            oadd("sw $ra, 0($sp)");
            oadd("jal " + getFunctionName(ir->call.functionName));
            oadd("lw $ra, 0($sp)");
            oadd("addi $sp, $sp, 4");
            if (curFunction == ir->call.functionName) {
                r1 = newReg();
                for (int i = varstmp.size() - 1; i >= 0; --i) {
                    r1 = mksur(varstmp[i], r1);
                    oadd("addi $3, $3, -4");
                    oadd("lw " + r1 + ", 0($3)");
                    save(varstmp[i], r1);
                    regc--;
                    regc--;
                }
                regc--;
            }
            r1 = mksur(irGetVariable(ir->call.ret), newReg());
            oadd("move " + r1 + ", $2");
            save(irGetVariable(ir->call.ret), r1);
            break;
        case PARAM:
            r1 = mksur(irGetVariable(ir->param), newReg());
            r2 = newReg();
            oadd("lw " + r2 + ", 0($sp)");
            oadd("addi $sp, $sp, 4");
            oadd("lw " + r1 + ", 0($sp)");
            oadd("addi $sp, $sp, 4");
            oadd("addi $sp, $sp, -4");
            oadd("sw " + r2 + ", 0($sp)");
            save(irGetVariable(ir->param), r1);
            break;
        case READ:
            oadd("addi $sp, $sp, -4");
            oadd("sw $ra, 0($sp)");
            oadd("jal read");
            oadd("lw $ra, 0($sp)");
            oadd("addi $sp, $sp, 4");
            r1 = mksur(ir->rw);
            oadd("move " + r1 + ", $v0");
            save(irGetVariable(ir->rw->var), r1);
            break;
        case WRITE:
            r1 = mksur(ir->rw);
            oadd("move $a0, " + r1);
            oadd("addi $sp, $sp, -4");
            oadd("sw $ra, 0($sp)");
            oadd("jal write");
            oadd("lw $ra, 0($sp)");
            oadd("addi $sp, $sp, 4");
            break;
        default:
            break;
        }
    }
    for (string out : outs) {
        fprintf(fp, "%s\n", ((out[0] == '.' || out.find(':') != string::npos ? "" : "  ") + out).c_str());
    }
}
