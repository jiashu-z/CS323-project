%{
#include "lexer.cpp"
#include "SyntaxTreeNode.h"

SyntaxTreeNode* root_node;

void yyerror(const char* s){
    has_error = true;
    printf("Error type B at Line %d: ", yylineno);
}
%}

%locations

%union{
    SyntaxTreeNode* node_type;
}

%nonassoc <node_type> LOWER_THAN_ELSE
%nonassoc <node_type> ELSE
%token <node_type> TYPE STRUCT
%token <node_type> IF WHILE RETURN
%token <node_type> CHAR
%token <node_type> INT
%token <node_type> FLOAT
%token <node_type> ID
%right <node_type> ASSIGN
%left <node_type> OR
%left <node_type> AND
%left <node_type> NE EQ GE GT LE LT
%left <node_type> MINUS PLUS
%left <node_type> MUL DIV
%right <node_type> NOT //todo: negative
%left <node_type> LP RP LB RB DOT //todo: correct or not
%token <node_type> SEMI COMMA
%token <node_type> LC RC

%type <node_type> Program ExtDefList
%type <node_type> ExtDef ExtDecList Specifier StructSpecifier VarDec
%type <node_type> FunDec VarList ParamDec CompSt StmtList Stmt DefList
%type <node_type> Def DecList Dec Args Exp

%nonassoc <node_type> ERROR

%%
// high-level definitions
Program: ExtDefList {
    $$ = new SyntaxTreeNode("Program",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
    $$->insert($1);
    root_node=$$;
};
ExtDefList:  {	$$ = new SyntaxTreeNode("ExtDefList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY); }
    | ExtDef ExtDefList {$$ = new SyntaxTreeNode("ExtDefList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);$$->insert({$1,$2});};

ExtDef: Specifier ExtDecList SEMI {$$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);$$->insert({$1,$2,$3});}
    | Specifier ExtDecList error {printf("Missing semicolon1 ';'\n");}
    | error ExtDecList SEMI {printf("Missing specifier\n");}
    | Specifier SEMI {$$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | error SEMI {printf("Missing specifier\n");}
    | Specifier error {printf("Missing semicolon2 ';'\n");}
    | Specifier FunDec CompSt {$$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | error FunDec CompSt {printf("Missing specifier\n");}
    ;
ExtDecList: VarDec {$$=new SyntaxTreeNode("ExtDecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | VarDec COMMA ExtDecList{$$=new SyntaxTreeNode("ExtDecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    ;

// specifier
Specifier: TYPE {$$=new SyntaxTreeNode("Specifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | StructSpecifier {$$=new SyntaxTreeNode("Specifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    ;
StructSpecifier: STRUCT ID LC DefList RC {$$=new SyntaxTreeNode("StructSpecifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4,$5});}
    | STRUCT ID{$$=new SyntaxTreeNode("StructSpecifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    ;
// declarator
VarDec: ID {$$=new SyntaxTreeNode("VarDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | VarDec LB INT RB {$$=new SyntaxTreeNode("VarDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    ;
FunDec: ID LP VarList RP {$$=new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | ID LP VarList error {printf("Missing closing parenthesis ')'\n");}
    | ID LP RP {$$=new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | ID LP error {printf("Missing closing parenthesis ')'\n");}
    ;
VarList: ParamDec COMMA VarList {$$=new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | ParamDec {$$=new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    ;
ParamDec: Specifier VarDec{$$=new SyntaxTreeNode("ParamDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);}
    | error VarDec {printf("Missing specifier\n");}
    ;

// statement
CompSt: LC DefList StmtList RC{$$=new SyntaxTreeNode("CompSt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);$$->insert($4);}
    ;
StmtList: {$$=new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY); }
    | Stmt StmtList{$$=new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    ;
Stmt: Exp SEMI {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);}
    | Exp error {printf("Missing semicolon ';'\n");}
    | CompSt{$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | RETURN Exp SEMI   {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | RETURN Exp error {printf("Missing semicolon ';'\n");}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4,$5});
    //TODO: not sure the %prec and lower_than_else usage
    }
    | IF LP Exp RP Stmt ELSE Stmt{
    $$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);$$->insert($4);$$->insert($5);$$->insert($6);$$->insert($7);
    }
    | IF LP Exp error Stmt {printf("Missing closing parenthesis ')'\n");}
    | WHILE LP Exp RP Stmt {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4,$5});}
    | WHILE LP Exp error Stmt {printf("Missing closing parenthesis ')'\n");}
    ;
// local definition
DefList: {$$=new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY); }
    | Def DefList {$$=new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    ;
Def: Specifier DecList SEMI{$$=new SyntaxTreeNode("Def",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Specifier DecList error {printf("Missing semicolon ';'\n");}
    | error DecList SEMI {printf("Missing specifier\n");}
    ;
DecList: Dec{$$=new SyntaxTreeNode("DecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | Dec COMMA DecList{$$=new SyntaxTreeNode("DecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    ;
Dec: VarDec{$$=new SyntaxTreeNode("Dec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | VarDec ASSIGN Exp{$$=new SyntaxTreeNode("Dec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    ;

// expression
Exp: Exp ASSIGN Exp {$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | Exp AND Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp OR Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp LT Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp LE Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp GT Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | Exp GE Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp NE Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp EQ Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp PLUS Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp MINUS Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp MUL Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp DIV Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | LP Exp RP{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | LP Exp error{printf("Missing closing parenthesis ')'\n");}
    | MINUS Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | NOT Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | ID LP Args RP{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | ID LP Args error {printf("Missing closing parenthesis ')'\n");}
    | ID LP RP{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | ID LP error{printf("Missing closing parenthesis ')'\n");}
    | Exp LB Exp RB{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | Exp DOT ID{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | ID {$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | INT { $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);  $$->insert($1); }
    | FLOAT{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | CHAR {$$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);  $$->insert($1);}
    | Exp ERROR Exp {}
    | ERROR {}
    ;
Args: Exp COMMA Args{$$=new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp{$$=new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    ;
%%
