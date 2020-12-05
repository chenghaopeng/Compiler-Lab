%locations

%{
    #include "stdio.h"
    #include "lex.yy.c"
    void yyerror(const char* msg);
    int yyparse(void);
    void yyread(char *fn) {
        yyin = fopen(fn, "r");
    }
%}

%token INT FLOAT
%token TYPE ID
%token SEMI COMMA
%token ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN
%token IF ELSE
%token WHILE

%right ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program : ExtDefList { printf("Program\n"); }
    ;
ExtDefList : 
    | ExtDef ExtDefList { printf("ExtDefList\n"); }
    ;
ExtDef : Specifier ExtDecList SEMI { printf("ExtDef\n"); }
    | Specifier SEMI { printf("ExtDef\n"); }
    | Specifier FunDec CompSt { printf("ExtDef\n"); }
    ;
ExtDecList : VarDec { printf("ExtDecList\n"); }
    | VarDec COMMA ExtDecList { printf("ExtDecList\n"); }
    ;
Specifier : TYPE { printf("Specifier\n"); }
    | StructSpecifier { printf("Specifier\n"); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { printf("StructSpecifier\n"); }
    | STRUCT Tag { printf("StructSpecifier\n"); }
    ;
OptTag :
    | ID { printf("OptTag\n"); }
    ;
Tag : ID { printf("Tag\n"); }
    ;
VarDec : ID { printf("VarDec\n"); }
    | VarDec LB INT RB { printf("VarDec\n"); }
    ;
FunDec : ID LP VarList RP { printf("FunDec\n"); }
    | ID LP RP { printf("FunDec\n"); }
    ;
VarList : ParamDec COMMA VarList { printf("VarList\n"); }
    | ParamDec { printf("VarList\n"); }
    ;
ParamDec : Specifier VarDec { printf("ParamDec\n"); }
    ;
CompSt : LC DefList StmtList RC { printf("CompSt\n"); }
    ;
StmtList : 
    | Stmt StmtList { printf("StmtList\n"); }
    ;
Stmt : Exp SEMI { printf("Stmt\n"); }
    | CompSt { printf("Stmt\n"); }
    | RETURN Exp SEMI { printf("Stmt\n"); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { printf("Stmt\n"); }
    | IF LP Exp RP Stmt ELSE Stmt { printf("Stmt\n"); }
    | WHILE LP Exp RP Stmt { printf("Stmt\n"); }
    ;
DefList : 
    | Def DefList { printf("DefList\n"); }
    ;
Def : Specifier DecList SEMI { printf("Def\n"); }
    ;
DecList : Dec { printf("DecList\n"); }
    | Dec COMMA DecList { printf("DecList\n"); }
    ;
Dec : VarDec { printf("Dec\n"); }
    | VarDec ASSIGNOP Exp { printf("Dec\n"); }
    ;
Exp : Exp ASSIGNOP Exp { printf("Exp\n"); }
    | Exp AND Exp { printf("Exp\n"); }
    | Exp OR Exp { printf("Exp\n"); }
    | Exp RELOP Exp { printf("Exp\n"); }
    | Exp PLUS Exp { printf("Exp\n"); }
    | Exp MINUS Exp { printf("Exp\n"); }
    | Exp STAR Exp { printf("Exp\n"); }
    | Exp DIV Exp { printf("Exp\n"); }
    | LP Exp RP { printf("Exp\n"); }
    | MINUS Exp { printf("Exp\n"); }
    | NOT Exp { printf("Exp\n"); }
    | ID LP Args RP { printf("Exp\n"); }
    | ID LP RP { printf("Exp\n"); }
    | Exp LB Exp RB { printf("Exp\n"); }
    | Exp DOT ID { printf("Exp\n"); }
    | ID { printf("Exp\n"); }
    | INT { printf("Exp\n"); }
    | FLOAT { printf("Exp\n"); }
    ;
Args : Exp COMMA Args { printf("Args\n"); }
    | Exp { printf("Args\n"); }
    ;