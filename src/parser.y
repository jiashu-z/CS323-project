%{
#include "lexer.cpp"
#include "SyntaxTreeNode.hh"
int isError=0;
SyntaxTreeNode* root_node;
#define PARSER_error_OUTPUT stdout
//#ifndef YYDEBUG
//# define YYDEBUG 1
//#endif
void yyerror(const char* s){
std::cout<<s<<std::endl;
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
    | Specifier SEMI {$$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | Specifier FunDec CompSt {$$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
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
    | VarDec LB INT RB{$$=new SyntaxTreeNode("VarDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    ;
FunDec: ID LP VarList RP{$$=new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | ID LP RP{$$=new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    ;
VarList: ParamDec COMMA VarList {$$=new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | ParamDec {$$=new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    ;
ParamDec: Specifier VarDec{$$=new SyntaxTreeNode("ParamDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);}
    ;

// statement
CompSt: LC DefList StmtList RC{$$=new SyntaxTreeNode("CompSt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);$$->insert($4);}
    ;
StmtList: {$$=new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY); }
    | Stmt StmtList{$$=new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    ;
Stmt: Exp SEMI {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);}
    | CompSt{$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | RETURN Exp SEMI   {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4,$5});
    //TODO: not sure the %prec and lower_than_else usage
    }
    | IF LP Exp RP Stmt ELSE Stmt{
    $$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);$$->insert($2);$$->insert($3);$$->insert($4);$$->insert($5);$$->insert($6);$$->insert($7);
    }
    | WHILE LP Exp RP Stmt{$$=new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4,$5});}
    ;
// local definition
DefList: {
$$=new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY); }
    | Def DefList {$$=new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    ;
Def: Specifier DecList SEMI{$$=new SyntaxTreeNode("Def",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
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
    | MINUS Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | NOT Exp{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2});}
    | ID LP Args RP{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | ID LP RP{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp LB Exp RB{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3,$4});}
    | Exp DOT ID{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | ID {$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | INT { $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);  $$->insert($1); }
    | FLOAT{$$=new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    | CHAR {$$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);  $$->insert($1);}
    ;
Args: Exp COMMA Args{$$=new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert({$1,$2,$3});}
    | Exp{$$=new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal); $$->insert($1);}
    ;
%%
