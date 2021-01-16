#include "global.h"

vector<AstNode>* ptrNodes;
vector<Symbol*> symbolList;
vector<InterRepresentation*> irList;

void init (vector<AstNode>* _nodes) {
    ptrNodes = _nodes;
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
    irPrint(stdout, ir);
}

void irPrint (FILE* fp) {
    for (InterRepresentation* ir : irList) {
        irPrint(fp, ir);
    }
}

void irPrint (FILE* fp, InterRepresentation* ir) {
    switch (ir->kind) {
    case LABEL:
        fprintf(fp, "LABEL label%d :\n", ir->labelId);
        break;
    case FUNCTION:
        fprintf(fp, "FUNCTION %s :\n", ir->functionName.c_str());
        break;
    case ASSIGN:
        fprintf(fp, "%s := %s\n", irGetOperand(ir->assign.left).c_str(), irGetOperand(ir->assign.right).c_str());
        break;
    case ADD:
        fprintf(fp, "%s := %s + %s\n", irGetOperand(ir->binaryAssign.left).c_str(), irGetOperand(ir->binaryAssign.op1).c_str(), irGetOperand(ir->binaryAssign.op2).c_str());
        break;
    case SUB:
        fprintf(fp, "%s := %s - %s\n", irGetOperand(ir->binaryAssign.left).c_str(), irGetOperand(ir->binaryAssign.op1).c_str(), irGetOperand(ir->binaryAssign.op2).c_str());
        break;
    case DIV:
        fprintf(fp, "%s := %s / %s\n", irGetOperand(ir->binaryAssign.left).c_str(), irGetOperand(ir->binaryAssign.op1).c_str(), irGetOperand(ir->binaryAssign.op2).c_str());
        break;
    case MUL:
        fprintf(fp, "%s := %s * %s\n", irGetOperand(ir->binaryAssign.left).c_str(), irGetOperand(ir->binaryAssign.op1).c_str(), irGetOperand(ir->binaryAssign.op2).c_str());
        break;
    case GOTO:
        fprintf(fp, "GOTO label%d\n", ir->gotoId);
        break;
    case IF_GOTO:
        fprintf(fp, "IF %s %s %s GOTO label%d\n", irGetOperand(ir->if_goto.left).c_str(), irGetRelop(ir->if_goto.kind), irGetOperand(ir->if_goto.right).c_str(), ir->if_goto.gotoId);
        break;
    case RETURN:
        fprintf(fp, "RETURN %s\n", irGetOperand(ir->returnVal).c_str());
        break;
    case DEC:
        fprintf(fp, "DEC %s %d\n", irGetVariable(ir->dec.var).c_str(), ir->dec.size);
        break;
    case ARG:
        fprintf(fp, "ARG %s\n", irGetOperand(ir->arg).c_str());
        break;
    case CALL:
        fprintf(fp, "%s := CALL %s\n", irGetVariable(ir->call.ret).c_str(), ir->call.functionName.c_str());
        break;
    case PARAM:
        fprintf(fp, "PARAM %s\n", irGetVariable(ir->param).c_str());
        break;
    case READ:
        fprintf(fp, "READ %s\n", irGetOperand(ir->rw).c_str());
        break;
    case WRITE:
        fprintf(fp, "WRITE %s\n", irGetOperand(ir->rw).c_str());
        break;
    default:
        break;
    }
}

string irGetOperand (Operand* operand) {
    switch (operand->kind) {
    case VARIABLE:
        return irGetVariable(operand->var);
        break;
    case CONSTANT:
        return "#" + to_string(operand->constVal);
        break;
    case REF:
        return "&" + irGetOperand(operand->ref);
        break;
    case DEREF:
        return "*" + irGetOperand(operand->deref);
        break;
    default:
        break;
    }
}

string irGetVariable (Variable* variable) {
    return string(variable->kind == V ? "v" : "t") + to_string(variable->id);
}
