void translateProgram (int);
void translateExtDefList (int);
void translateExtDef (int);
void translateFunDec (int);
void translateVarList (int);
void translateParamDec (int);
void translateCompSt (int);
void translateStmtList (int);
void translateStmt (int);
void translateDefList (int);
void translateDef (int);
void translateDecList (int);
void translateDec (int);
Operand* translateVarDec (int);
Operand* translateExp (int);
void translateCondExp (int, int, int);
void translateArgs (int);