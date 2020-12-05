%locations

%{
    #include "stdio.h"
    #include "lex.yy.c"
    #include "io.h"
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
Program : ExtDefList { push_str("Program"); }
    ;
ExtDefList : 
    | ExtDef ExtDefList { push_str("ExtDefList"); }
    ;
ExtDef : Specifier ExtDecList SEMI { push_str("ExtDef"); }
    | Specifier SEMI { push_str("ExtDef"); }
    | Specifier FunDec CompSt { push_str("ExtDef"); }
    ;
ExtDecList : VarDec { push_str("ExtDecList"); }
    | VarDec COMMA ExtDecList { push_str("ExtDecList"); }
    ;
Specifier : TYPE { push_str("Specifier"); }
    | StructSpecifier { push_str("Specifier"); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { push_str("StructSpecifier"); }
    | STRUCT Tag { push_str("StructSpecifier"); }
    ;
OptTag :
    | ID { push_str("OptTag"); }
    ;
Tag : ID { push_str("Tag"); }
    ;
VarDec : ID { push_str("VarDec"); }
    | VarDec LB INT RB { push_str("VarDec"); }
    ;
FunDec : ID LP VarList RP { push_str("FunDec"); }
    | ID LP RP { push_str("FunDec"); }
    ;
VarList : ParamDec COMMA VarList { push_str("VarList"); }
    | ParamDec { push_str("VarList"); }
    ;
ParamDec : Specifier VarDec { push_str("ParamDec"); }
    ;
CompSt : LC DefList StmtList RC { push_str("CompSt"); }
    ;
StmtList : 
    | Stmt StmtList { push_str("StmtList"); }
    ;
Stmt : Exp SEMI { push_str("Stmt"); }
    | CompSt { push_str("Stmt"); }
    | RETURN Exp SEMI { push_str("Stmt"); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { push_str("Stmt"); }
    | IF LP Exp RP Stmt ELSE Stmt { push_str("Stmt"); }
    | WHILE LP Exp RP Stmt { push_str("Stmt"); }
    ;
DefList : 
    | Def DefList { push_str("DefList"); }
    ;
Def : Specifier DecList SEMI { push_str("Def"); }
    ;
DecList : Dec { push_str("DecList"); }
    | Dec COMMA DecList { push_str("DecList"); }
    ;
Dec : VarDec { push_str("Dec"); }
    | VarDec ASSIGNOP Exp { push_str("Dec"); }
    ;
Exp : Exp ASSIGNOP Exp { push_str("Exp"); }
    | Exp AND Exp { push_str("Exp"); }
    | Exp OR Exp { push_str("Exp"); }
    | Exp RELOP Exp { push_str("Exp"); }
    | Exp PLUS Exp { push_str("Exp"); }
    | Exp MINUS Exp { push_str("Exp"); }
    | Exp STAR Exp { push_str("Exp"); }
    | Exp DIV Exp { push_str("Exp"); }
    | LP Exp RP { push_str("Exp"); }
    | MINUS Exp { push_str("Exp"); }
    | NOT Exp { push_str("Exp"); }
    | ID LP Args RP { push_str("Exp"); }
    | ID LP RP { push_str("Exp"); }
    | Exp LB Exp RB { push_str("Exp"); }
    | Exp DOT ID { push_str("Exp"); }
    | ID { push_str("Exp"); }
    | INT { push_str("Exp"); }
    | FLOAT { push_str("Exp"); }
    ;
Args : Exp COMMA Args { push_str("Args"); }
    | Exp { push_str("Args"); }
    ;