%locations

%{
    #include "stdio.h"
    #include "lex.yy.c"
    void yyerror(const char* msg);
    int yyparse(void);
    void yyread(char *fn) {
        yyin = fopen(fn, "r");
    }
    void push_str(char*);
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
%left AND OR
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program : ExtDefList { push_str("Program"); }
    ;
ExtDefList : 
    | ExtDef ExtDefList { push_str("ExtDefList"); push_str("ExtDef"); push_str("ExtDefList"); }
    ;
ExtDef : Specifier ExtDecList SEMI { push_str("ExtDef"); push_str("Specifier"); push_str("ExtDecList"); push_str("SEMI"); }
    | Specifier SEMI { push_str("ExtDef"); push_str("Specifier"); push_str("SEMI"); }
    | Specifier FunDec CompSt { push_str("ExtDef"); push_str("Specifier"); push_str("FunDec"); push_str("CompSt"); }
    ;
ExtDecList : VarDec { push_str("ExtDecList"); push_str("VarDec"); }
    | VarDec COMMA ExtDecList { push_str("ExtDecList"); push_str("VarDec"); push_str("COMMA"); push_str("ExtDecList"); }
    ;
Specifier : TYPE { push_str("Specifier"); push_str("TYPE"); }
    | StructSpecifier { push_str("Specifier"); push_str("StructSpecifier"); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { push_str("StructSpecifier"); push_str("STRUCT"); push_str("OptTag"); push_str("LC"); push_str("DefList"); push_str("RC"); }
    | STRUCT Tag { push_str("StructSpecifier"); push_str("STRUCT"); push_str("Tag"); }
    ;
OptTag :
    | ID { push_str("OptTag"); push_str("ID"); }
    ;
Tag : ID { push_str("Tag"); push_str("ID"); }
    ;
VarDec : ID { push_str("VarDec"); push_str("ID"); }
    | VarDec LB INT RB { push_str("VarDec"); push_str("VarDec"); push_str("LB"); push_str("INT"); push_str("RB"); }
    ;
FunDec : ID LP VarList RP { push_str("FunDec"); push_str("ID"); push_str("LP"); push_str("VarList"); push_str("RP"); }
    | ID LP RP { push_str("FunDec"); push_str("ID"); push_str("LP"); push_str("RP"); }
    ;
VarList : ParamDec COMMA VarList { push_str("VarList"); push_str("ParamDec"); push_str("COMMA"); push_str("VarList"); }
    | ParamDec { push_str("VarList"); push_str("ParamDec"); }
    ;
ParamDec : Specifier VarDec { push_str("ParamDec"); push_str("Specifier"); push_str("VarDec"); }
    ;
CompSt : LC DefList StmtList RC { push_str("CompSt"); push_str("LC"); push_str("DefList"); push_str("StmtList"); push_str("RC"); }
    ;
StmtList : 
    | Stmt StmtList { push_str("StmtList"); push_str("Stmt"); push_str("StmtList"); }
    ;
Stmt : Exp SEMI { push_str("Stmt"); push_str("Exp"); push_str("SEMI"); }
    | CompSt { push_str("Stmt"); push_str("CompSt"); }
    | RETURN Exp SEMI { push_str("Stmt"); push_str("RETURN"); push_str("Exp"); push_str("SEMI"); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { push_str("Stmt"); push_str("IF"); push_str("LP"); push_str("Exp"); push_str("RP"); push_str("Stmt"); }
    | IF LP Exp RP Stmt ELSE Stmt { push_str("Stmt"); push_str("IF"); push_str("LP"); push_str("Exp"); push_str("RP"); push_str("Stmt"); push_str("ELSE"); push_str("Stmt"); }
    | WHILE LP Exp RP Stmt { push_str("Stmt"); push_str("WHILE"); push_str("LP"); push_str("Exp"); push_str("RP"); push_str("Stmt"); }
    ;
DefList : 
    | Def DefList { push_str("DefList"); push_str("Def"); push_str("DefList"); }
    ;
Def : Specifier DecList SEMI { push_str("Def"); push_str("Specifier"); push_str("DecList"); push_str("SEMI"); }
    ;
DecList : Dec { push_str("DecList"); push_str("Dec"); }
    | Dec COMMA DecList { push_str("DecList"); push_str("Dec"); push_str("COMMA"); push_str("DecList"); }
    ;
Dec : VarDec { push_str("Dec"); push_str("VarDec"); }
    | VarDec ASSIGNOP Exp { push_str("Dec"); push_str("VarDec"); push_str("ASSIGNOP"); push_str("Exp"); }
    ;
Exp : Exp ASSIGNOP Exp { push_str("Exp"); push_str("Exp"); push_str("ASSIGNOP"); push_str("Exp"); }
    | Exp AND Exp { push_str("Exp"); push_str("Exp"); push_str("AND"); push_str("Exp"); }
    | Exp OR Exp { push_str("Exp"); push_str("Exp"); push_str("OR"); push_str("Exp"); }
    | Exp RELOP Exp { push_str("Exp"); push_str("Exp"); push_str("RELOP"); push_str("Exp"); }
    | Exp PLUS Exp { push_str("Exp"); push_str("Exp"); push_str("PLUS"); push_str("Exp"); }
    | Exp MINUS Exp { push_str("Exp"); push_str("Exp"); push_str("MINUS"); push_str("Exp"); }
    | Exp STAR Exp { push_str("Exp"); push_str("Exp"); push_str("STAR"); push_str("Exp"); }
    | Exp DIV Exp { push_str("Exp"); push_str("Exp"); push_str("DIV"); push_str("Exp"); }
    | LP Exp RP { push_str("Exp"); push_str("LP"); push_str("Exp"); push_str("RP"); }
    | MINUS Exp { push_str("Exp"); push_str("MINUS"); push_str("Exp"); }
    | NOT Exp { push_str("Exp"); push_str("NOT"); push_str("Exp"); }
    | ID LP Args RP { push_str("Exp"); push_str("ID"); push_str("LP"); push_str("Args"); push_str("RP"); }
    | ID LP RP { push_str("Exp"); push_str("ID"); push_str("LP"); push_str("RP"); }
    | Exp LB Exp RB { push_str("Exp"); push_str("Exp"); push_str("LB"); push_str("Exp"); push_str("RB"); }
    | Exp DOT ID { push_str("Exp"); push_str("Exp"); push_str("DOT"); push_str("ID"); }
    | ID { push_str("Exp"); push_str("ID"); }
    | INT { push_str("Exp"); push_str("INT"); }
    | FLOAT { push_str("Exp"); push_str("FLOAT"); }
    ;
Args : Exp COMMA Args { push_str("Args"); push_str("Exp"); push_str("COMMA"); push_str("Args"); }
    | Exp { push_str("Args"); push_str("Exp"); }
    ;