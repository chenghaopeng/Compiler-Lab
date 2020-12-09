%locations

%{
    #include "stdio.h"
    #include "lex.yy.c"
    void yyerror(const char* msg);
    int yyparse(void);
    void yyread(char *fn) {
        yyin = fopen(fn, "r");
    }
    extern int newAstNode(char*, int, ...);
    extern int flag;
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
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program : ExtDefList { $$ = newAstNode("Program", 1, $1); }
    ;
ExtDefList : { $$ = newAstNode("ExtDefList", 0, -1); }
    | ExtDef ExtDefList { $$ = newAstNode("ExtDefList", 2, $1, $2); }
    ;
ExtDef : Specifier ExtDecList SEMI { $$ = newAstNode("ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI { $$ = newAstNode("ExtDef", 2, $1, $2); }
    | Specifier FunDec CompSt { $$ = newAstNode("ExtDef", 3, $1, $2, $3); }
    | Specifier error SEMI
    | Specifier error CompSt
    | Specifier FunDec error
    ;
ExtDecList : VarDec { $$ = newAstNode("ExtDecList", 1, $1); }
    | VarDec COMMA ExtDecList { $$ = newAstNode("ExtDecList", 3, $1, $2, $3); }
    ;
Specifier : TYPE { $$ = newAstNode("Specifier", 1, $1); }
    | StructSpecifier { $$ = newAstNode("Specifier", 1, $1); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { $$ = newAstNode("StructSpecifier", 5, $1, $2, $3, $4, $5); }
    | STRUCT Tag { $$ = newAstNode("StructSpecifier", 2, $1, $2); }
    | STRUCT OptTag LC error RC
    | STRUCT error LC DefList RC
    | STRUCT error LC error RC
    | STRUCT error RC
    ;
OptTag : { $$ = newAstNode("OptTag", 0, -1); }
    | ID { $$ = newAstNode("OptTag", 1, $1); }
    ;
Tag : ID { $$ = newAstNode("Tag", 1, $1); }
    ;
VarDec : ID { $$ = newAstNode("VarDec", 1, $1); }
    | VarDec LB INT RB { $$ = newAstNode("VarDec", 4, $1, $2, $3, $4); }
    | VarDec LB error RB
    ;
FunDec : ID LP VarList RP { $$ = newAstNode("FunDec", 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = newAstNode("FunDec", 3, $1, $2, $3); }
    | ID LP error RP
    ;
VarList : ParamDec COMMA VarList { $$ = newAstNode("VarList", 3, $1, $2, $3); }
    | ParamDec { $$ = newAstNode("VarList", 1, $1); }
    ;
ParamDec : Specifier VarDec { $$ = newAstNode("ParamDec", 2, $1, $2); }
    ;
CompSt : LC DefList StmtList RC { $$ = newAstNode("CompSt", 4, $1, $2, $3, $4); }
    | error RC
    ;
StmtList : { $$ = newAstNode("StmtList", 0, -1); }
    | Stmt StmtList { $$ = newAstNode("StmtList", 2, $1, $2); }
    ;
Stmt : Exp SEMI { $$ = newAstNode("Stmt", 2, $1, $2); }
    | CompSt { $$ = newAstNode("Stmt", 1, $1); }
    | RETURN Exp SEMI { $$ = newAstNode("Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = newAstNode("Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = newAstNode("Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$ = newAstNode("Stmt", 5, $1, $2, $3, $4, $5); }
    | error SEMI
    | RETURN error SEMI
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE
    | IF LP error RP Stmt ELSE Stmt
    | WHILE LP error RP Stmt
    ;
DefList : { $$ = newAstNode("DefList", 0, -1); }
    | Def DefList { $$ = newAstNode("DefList", 2, $1, $2); }
    ;
Def : Specifier DecList SEMI { $$ = newAstNode("Def", 3, $1, $2, $3); }
    | Specifier error SEMI
    ;
DecList : Dec { $$ = newAstNode("DecList", 1, $1); }
    | Dec COMMA DecList { $$ = newAstNode("DecList", 3, $1, $2, $3); }
    ;
Dec : VarDec { $$ = newAstNode("Dec", 1, $1); }
    | VarDec ASSIGNOP Exp { $$ = newAstNode("Dec", 3, $1, $2, $3); }
    ;
Exp : Exp ASSIGNOP Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp AND Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp OR Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp RELOP Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp PLUS Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp MINUS Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp STAR Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp DIV Exp { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | LP Exp RP { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | MINUS Exp { $$ = newAstNode("Exp", 2, $1, $2); }
    | NOT Exp { $$ = newAstNode("Exp", 2, $1, $2); }
    | ID LP Args RP { $$ = newAstNode("Exp", 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | Exp LB Exp RB { $$ = newAstNode("Exp", 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$ = newAstNode("Exp", 3, $1, $2, $3); }
    | ID { $$ = newAstNode("Exp", 1, $1); }
    | INT { $$ = newAstNode("Exp", 1, $1); }
    | FLOAT { $$ = newAstNode("Exp", 1, $1); }
    | LP error RP
    | ID LP error RP
    | Exp LB error RB
    ;
Args : Exp COMMA Args { $$ = newAstNode("Args", 3, $1, $2, $3); }
    | Exp { $$ = newAstNode("Args", 1, $1); }
    | error COMMA Args
    ;

%%

void yyerror (const char* msg) {
    flag = 0;
    fprintf(stderr, "Error type B at Line %d: %s, unexpected %s. yylval=%d\n", yylineno, msg, yytext, yylval);
}
