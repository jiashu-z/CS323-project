%{
#include "lexer.cpp"
#include "SyntaxTreeNode.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"

SymbolTable symbolTable;
SyntaxTreeNode* root_node;
int error_cnt = 0;

void error_message(const char* message){
    if (error_cnt > 0) {
        printf("%s\n", message);
        error_cnt -= 1;
    }
}

void yyerror(const char* s){
    has_error = true;
    error_message("Syntax error");
    error_cnt += 1;
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
%token <node_type> IF WHILE RETURN FOR
%token <node_type> CHAR
%token <node_type> INT
%token <node_type> FLOAT
%token <node_type> ID
%right <node_type> ASSIGN
%left <node_type> OR
%left <node_type> AND
%left <node_type> NE EQ GE GT LE LT
%nonassoc LOWER_NEGA
%left <node_type> MINUS PLUS
%left <node_type> MUL DIV
%right <node_type> NOT //todo: negative
%left <node_type> LP RP LB RB DOT
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
ExtDefList: ExtDef ExtDefList {
        $$ = new SyntaxTreeNode("ExtDefList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    }
    | {
        $$ = new SyntaxTreeNode("ExtDefList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY);
    }
    ;
ExtDef: Specifier ExtDecList SEMI {
        $$ = new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
	insertExtPrimarySymbol($$,symbolTable);
    }
    | Specifier SEMI {
        $$=new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1, $2});
    }
    | Specifier FunDec CompSt {
        $$ = new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
        insertFunctionSymbol($2,symbolTable,true);
        assignFunctionReturnType($1,$2,symbolTable);
        checkIfExistFunctionReturnStatement($1,$3,symbolTable);
    }
    | Specifier FunDec SEMI {
            $$ = new SyntaxTreeNode("ExtDef",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
            $$->insert($1);
            $$->insert($2);
            $$->insert($3);
    }
    | error SEMI {
        error_message("Missing specifier");
    }
    | ExtDecList error SEMI {
        error_message("Missing specifier");
    }
    | Specifier error {
        error_message("Missing semicolon ';'");
    }
    | error FunDec CompSt {
        error_message("Missing specifier");
    }
    ;
ExtDecList: VarDec {
        $$ = new SyntaxTreeNode("ExtDecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    }
    | VarDec COMMA ExtDecList {
        $$ = new SyntaxTreeNode("ExtDecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    }
    | VarDec ExtDecList error {
        error_message("Missing comma ','");
    }
    ;

// specifier
Specifier: TYPE {
        $$ = new SyntaxTreeNode("Specifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        assignSpecifierType($$);
    }
    | StructSpecifier {
        $$ = new SyntaxTreeNode("Specifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
	assignStructSpecifierType($$,symbolTable);
    }
    ;
StructSpecifier: STRUCT ID LC DefList RC {
        $$ = new SyntaxTreeNode("StructSpecifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4,$5});
        insertStructDefinitionSymbol($$, symbolTable);
    }
    | STRUCT ID LC DefList error {
        error_message("Missing right brace '}'");
    }
    | STRUCT ID {
        $$ = new SyntaxTreeNode("StructSpecifier",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    }
    ;
// declarator
VarDec: ID {
        $$ = new SyntaxTreeNode("VarDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    }
    | VarDec LB INT RB {
        $$ = new SyntaxTreeNode("VarDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4});
        assignIDArrayType($$,$1);
    }
    ;
FunDec: ID LP VarList RP {
        $$ = new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4});
        insertFunctionSymbol($$,symbolTable,false);
    }
    | ID LP RP {
        $$ = new SyntaxTreeNode("FunDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        insertFunctionSymbol($$,symbolTable,false);
    }
    | ID LP VarList error {
        error_message("Missing closing parenthesis ')'");
    }
    | ID LP error {
        error_message("Missing closing parenthesis ')'");
    }
    | ID error VarList RP {
        error_message("Missing left parenthesis ')'");
    }
    | ID error RP {
        error_message("Missing left parenthesis ')'");
    }
    ;
VarList: ParamDec COMMA VarList {
        $$ = new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
    }
    | ParamDec VarList error {
        error_message("Missing comma ','");
    }
    | ParamDec {
        $$ = new SyntaxTreeNode("VarList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    }
    ;
ParamDec: Specifier VarDec {
        $$ = new SyntaxTreeNode("ParamDec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
    }
    ;

// statement
CompSt: LC DefList StmtList RC{
        $$ = new SyntaxTreeNode("CompSt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
        $$->insert($4);
    }
    ;
StmtList: Stmt StmtList{
        $$ = new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    }
    | {
        $$ = new SyntaxTreeNode("StmtList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY);
    }
    ;
Stmt: Exp SEMI {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
    }
    | Exp error {
        error_message("Missing semicolon ';'");
    }
    | CompSt {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    }
    | RETURN Exp SEMI {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
    }
    | RETURN Exp error {
        error_message("Missing semicolon ';'");
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4,$5});
    }
    | IF LP Exp RP Stmt ELSE Stmt {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
        $$->insert($4);
        $$->insert($5);
        $$->insert($6);
        $$->insert($7);
    }
    | IF LP Exp error Stmt {
        error_message("Missing closing parenthesis ')'");
    }
    | WHILE LP Exp RP Stmt {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4,$5});
    }
    | WHILE LP Exp error Stmt {
        error_message("Missing closing parenthesis ')'");
    }
    | FOR LP Exp SEMI Exp SEMI Exp RP Stmt {
        $$ = new SyntaxTreeNode("Stmt",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4,$5,$6,$7,$8,$9});
    }
    | FOR LP Exp SEMI Exp SEMI Exp error Stmt {
        error_message("Missing closing parenthesis ')'");
    }
    | IF error Exp RP Stmt {
        error_message("Missing left parenthesis ')'");
    }
    | WHILE error Exp RP Stmt {
        error_message("Missing left parenthesis ')'");
    }
    | FOR error Exp SEMI Exp SEMI Exp RP Stmt {
        error_message("Missing left parenthesis ')'");
    }
    ;

// local definition
DefList: Def DefList {
        $$ = new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    }
    | {
        $$ = new SyntaxTreeNode("DefList",yytext,@$.first_line,@$.first_column,TreeNodeType::EMPTY);
    }
    ;
Def: Specifier DecList SEMI {
        $$ = new SyntaxTreeNode("Def",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        insertPrimarySymbol($$,symbolTable);
    }
    | Specifier DecList error {
        error_message("Missing semicolon ';'");
    }
DecList: Dec {
        $$ = new SyntaxTreeNode("DecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Dec COMMA DecList {
        $$ = new SyntaxTreeNode("DecList",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Dec DecList error {
        printf("Missing comma ','\n");
    }
    ;
Dec: VarDec {
        $$ = new SyntaxTreeNode("Dec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        updateSyntaxTreeNodeSymbol($$,$1);
    }
    | VarDec ASSIGN Exp {
        $$ = new SyntaxTreeNode("Dec",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        updateSyntaxTreeNodeSymbol($$,$1);
    };

// expression
Exp: Exp ASSIGN Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
        updateSyntaxTreeNodeSymbol($$,$1);
        checkAssignDataType($1,$3);
        checkrValue($1);
    }
    | Exp AND Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp OR Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
            updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp LT Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
           updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp LE Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
       updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp GT Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        $$->insert($2);
        $$->insert($3);
        updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp GE Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp NE Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp EQ Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp PLUS Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
        checkBinaryOperator($$,$1,$3);
    }
    | Exp MINUS Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp MUL Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp DIV Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | LP Exp RP {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    updateSyntaxTreeNodeSymbol($$,$2);
    }
    | MINUS Exp %prec LOWER_NEGA {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    updateSyntaxTreeNodeSymbol($$,$2);
    }
    | NOT Exp {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2});
    updateSyntaxTreeNodeSymbol($$,$2);
    }
    | ID LP Args RP {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4});
        getFunctionSymbol($$,symbolTable);
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | ID LP RP {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
        getFunctionSymbol($$,symbolTable);
    updateSyntaxTreeNodeSymbol($$,$1);
    }
    | Exp LB Exp RB {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3,$4});

        checkAndUpdateExpArray($$,$1,$3,symbolTable);
    }
    | Exp DOT ID {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
//        updateSyntaxTreeNodeSymbol($$,$1);
        checkDotOperator($$, $1, $3, symbolTable);
    }
    | ID {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
        getVariableSymbol($1,symbolTable);
        updateSyntaxTreeNodeSymbol($$, $1);
    }
    | INT {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    assignConstantSymbol($$,SymbolType::INT);
    }
    | FLOAT {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    assignConstantSymbol($$,SymbolType::FLOAT);
    }
    | CHAR {
        $$ = new SyntaxTreeNode("Exp",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    assignConstantSymbol($$,SymbolType::CHAR);
    }
    | ID LP error {
        printf("Missing closing parenthesis ')'\n");
    }
    | ID LP Args error {
        error_message("Missing closing parenthesis ')'");
    }
    | LP Exp error SEMI {
        printf("Missing closing parenthesis ')'\n");
    }
    | Exp ERROR Exp {}
    | ERROR {}
    ;
Args: Exp COMMA Args {
        $$ = new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert({$1,$2,$3});
    }
    | Exp {
        $$ = new SyntaxTreeNode("Args",yytext,@$.first_line,@$.first_column,TreeNodeType::Non_Terminal);
        $$->insert($1);
    }
    ;
%%
