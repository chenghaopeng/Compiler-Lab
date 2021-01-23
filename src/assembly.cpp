#include "global.h"
#include <map>

#define add outs.push_back

vector<string> outs;
map<string, bool> inData;

int regc = 0;

string newReg () {
    return "$" + to_string(regc++);
}

string mksur (string var, string reg) {
    if (!inData[var]) {
        inData[var] = true;
        outs.insert(outs.begin() + 2, var + ": .space 4");
    }
    string tmp = newReg();
    add("la " + tmp + ", " + var);
    add("lw " + reg + ", 0(" + tmp + ")");
    return reg;
}

string mksur (Operand* operand) {
    string var, reg = newReg(), tmp;
    switch (operand->kind) {
    case VARIABLE:
        mksur(irGetVariable(operand->var), reg);
        break;
    case CONSTANT:
        add("li " + reg + ", " + to_string(operand->constVal));
        break;
    case REF:
        var = irGetVariable(operand->ref->var);
        add("la " + reg + ", " + var);
        break;
    case DEREF:
        var = irGetVariable(operand->deref->var);
        tmp = newReg();
        add("la " + tmp + ", " + var);
        add("lw " + reg + ", 0(" + tmp + ")");
        add("lw " + reg + ", 0(" + reg + ")");
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
        add("la " + tmp + ", " + var);
        add("sw " + reg + ", 0(" + tmp + ")");
        break;
    case CONSTANT:
        break;
    case REF:
        break;
    case DEREF:
        var = irGetVariable(operand->deref->var);
        tmp = newReg();
        add("la " + tmp + ", " + var);
        add("lw " + reg + ", 0(" + tmp + ")");
        add("sw " + reg + ", 0(" + reg + ")");
        break;
    default:
        break;
    }
}

void save (string var, string reg) {
    string tmp = newReg();
    add("la " + tmp + ", " + var);
    add("sw " + reg + ", 0(" + tmp + ")");
}

string getLabel (int id) {
    return "label" + to_string(id);
}

void asmPrint (FILE* fp) {
    add(".data");
    add("_prompt: .asciiz \"Enter an integer:\"");
    add(".globl main");
    add(".text");
    add("read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n\nwrite:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");
    string r1, r2, r3;
    bool isMain = false;
    for (int i = 0; i < irCount(); ++i) {
        InterRepresentation* ir = irGet(i);
        regc = 8;
        switch (ir->kind) {
        case LABEL:
            add(getLabel(ir->labelId) + ":");
            break;
        case FUNCTION:
            add(ir->functionName + ":");
            if (ir->functionName == "main") {
                isMain = true;
            }
            else {
                isMain = false;
            }
            break;
        case ASSIGN:
            r1 = mksur(ir->assign.left);
            r2 = mksur(ir->assign.right);
            add("move " + r1 + ", " + r2);
            save(ir->assign.left, r1);
            break;
        case ADD:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            add("add " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case SUB:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            add("sub " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case DIV:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            add("div " + r2 + ", " + r3);
            add("mflo " + r1);
            save(ir->binaryAssign.left, r1);
            break;
        case MUL:
            r1 = mksur(ir->binaryAssign.left);
            r2 = mksur(ir->binaryAssign.op1);
            r3 = mksur(ir->binaryAssign.op2);
            add("mul " + r1 + ", " + r2 + ", " + r3);
            save(ir->binaryAssign.left, r1);
            break;
        case GOTO:
            add("j " + getLabel(ir->gotoId));
            break;
        case IF_GOTO:
            r1 = mksur(ir->if_goto.left);
            r2 = mksur(ir->if_goto.right);
            r3 = asmGetRelop(ir->if_goto.kind);
            add(r3 + " " + r1 + ", " + r2 + ", " + getLabel(ir->if_goto.gotoId));
            break;
        case RETURN:
            r1 = mksur(ir->returnVal);
            add("move $2, " + r1);
            if (!isMain) {
                add("jr $ra");
            }
            break;
        case DEC:
            r1 = irGetVariable(ir->dec.var);
            inData[r1] = true;
            outs.insert(outs.begin() + 2, r1 + ": .space " + to_string(ir->dec.size));
            break;
        case ARG:
            r1 = mksur(ir->arg);
            add("sw " + r1 + ", 0($sp)");
            add("addi $sp, $sp, 4");
            break;
        case CALL:
            r1 = mksur(irGetVariable(ir->call.ret), newReg());
            add("sw $ra, 0($sp)");
            add("addi $sp, $sp, 4");
            add("jal " + ir->call.functionName);
            add("addi $sp, $sp, -4");
            add("lw $ra, 0($sp)");
            add("move " + r1 + ", $2");
            save(irGetVariable(ir->call.ret), r1);
            break;
        case PARAM:
            r1 = mksur(irGetVariable(ir->param), newReg());
            r2 = newReg();
            add("addi $sp, $sp, -4");
            add("lw " + r2 + ", 0($sp)");
            add("addi $sp, $sp, -4");
            add("lw " + r1 + ", 0($sp)");
            add("sw " + r2 + ", 0($sp)");
            add("addi $sp, $sp, 4");
            break;
        case READ:
            r1 = mksur(ir->rw);
            add("sw $ra, 0($sp)");
            add("addi $sp, $sp, 4");
            add("jal read");
            add("addi $sp, $sp, -4");
            add("lw $ra, 0($sp)");
            add("move " + r1 + ", $2");
            save(irGetVariable(ir->call.ret), r1);
            break;
        case WRITE:
            r1 = mksur(ir->rw);
            add("sw " + r1 + ", 0($sp)");
            add("addi $sp, $sp, 4");
            add("sw $ra, 0($sp)");
            add("addi $sp, $sp, 4");
            add("jal write");
            add("addi $sp, $sp, -4");
            add("lw $ra, 0($sp)");
            break;
        default:
            break;
        }
    }
    for (string out : outs) {
        fprintf(fp, "%s\n", out.c_str());
    }
}
