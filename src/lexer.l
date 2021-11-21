/*https://stackoverflow.com/questions/15550553/flex-regular-expression-literal-char*/
%x incl
%{
#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 1;
  int yycolumn = 1;
  #define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng;\
    yycolumn += yyleng;

  #include <iostream>
  #include <vector>
  #include "SyntaxTreeNode.h"
  #include "parser.h"

  int line_number = 1;
  bool has_error = false;
  extern "C" int yylex();
%}

%option yylineno

CHAR '.'|'\\x[0-9a-fA-F]{1,2}'
INVALID_CHAR ''|'[^']{2,}'

INT [0-9]+
HEX_INT 0[xX](0|[1-9a-fA-F][0-9a-fA-F]*)
INVALID_INT_START_WITH_0 0[0-9]+
FLOAT {digit}+\.{digit}+
TYPE int|float|char
STRUCT struct
IF if
ELSE else
WHILE while
FOR for
RETURN return
DOT \.
SEMI ;
COMMA ,
ASSIGN =
LT <
LE <=
GT >
GE >=
NE !=
EQ ==
PLUS \+
MINUS -
MUL \*
DIV \/
AND &&
OR \|\|
NOT !
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
letter [a-zA-Z]
letter_ {letter}|_
digit [0-9]
INVALID_HEX_CHAR ('\\x[0-9a-fA-F]*[g-zG-Z][0-9a-fA-F]*')|('\\x[0-9a-fA-F]{3,}')

%%
{INVALID_HEX_CHAR} {
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ERROR;
}
#include BEGIN(incl);
<incl>[ \t]* {/* ignore white space */}
<incl>[^ \t\n]+ { /* read another file */
    char *buffer;
    if ((buffer = getcwd(NULL, 0)) == NULL) {
        perror("getcwd error");
        exit(-1);
    }
    char path[200];
    sprintf(path,"%s/test-ex/%s", buffer, yytext);
    if (include_stack_ptr >= MAX_INCLUDE_DEPTH) {
        fprintf(stderr,"Includes nested too deeply");
        exit(1);
    }
    include_stack[include_stack_ptr] =
            YY_CURRENT_BUFFER;
    include_stack_ptr++;
    yyin = fopen(path, "r");
    if (!yyin) {
        printf("can not open:%s\n", path);
    }
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    BEGIN(INITIAL);
};
<<EOF>> {
        include_stack_ptr--;
        if (include_stack_ptr==0)
        {
            yyterminate();

        }
        else
        {
            yy_delete_buffer(YY_CURRENT_BUFFER);
            yy_switch_to_buffer(
                    include_stack[include_stack_ptr]);
        }
}


{CHAR} {
  yylval.node_type=new SyntaxTreeNode("char_constant",yytext,line_number,TreeNodeType::CHAR);
  return CHAR;
}
{INVALID_CHAR} {
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ERROR;
}

{INVALID_INT_START_WITH_0} {
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ERROR;
}
{INT} {

  yylval.node_type=new SyntaxTreeNode("int_constant",yytext,line_number,TreeNodeType::INT);
  return INT;
}
{HEX_INT} {
  yylval.node_type=new SyntaxTreeNode("int_constant",strtol(yytext, NULL, 16),line_number,TreeNodeType::INT);
  return INT;
}
{FLOAT} {
  yylval.node_type=new SyntaxTreeNode("float_constant",yytext,line_number,TreeNodeType::FLOAT);
  return FLOAT;
}
{TYPE} {
  yylval.node_type=new SyntaxTreeNode("TYPE",yytext,line_number,TreeNodeType::TYPE);
  return TYPE;
}
{STRUCT} {
  yylval.node_type=new SyntaxTreeNode("STRUCT",yytext,line_number);
  return STRUCT;
}
{IF} {
  yylval.node_type=new SyntaxTreeNode("IF",yytext,line_number);
  return IF;
}
{ELSE} {
  yylval.node_type=new SyntaxTreeNode("ELSE",yytext,line_number);
  return ELSE;
}
{WHILE} {
  yylval.node_type=new SyntaxTreeNode("WHILE",yytext,line_number);
  return WHILE;
}
{FOR} {
  yylval.node_type=new SyntaxTreeNode("FOR",yytext,line_number);
  return FOR;
}
{RETURN} {
  yylval.node_type=new SyntaxTreeNode("RETURN",yytext,line_number);
  return RETURN;
}
{DOT} {
  yylval.node_type=new SyntaxTreeNode("DOT",yytext,line_number);
  return DOT;
}
{SEMI} {
  yylval.node_type=new SyntaxTreeNode("SEMI",yytext,line_number);
  return SEMI;
}
{COMMA} {
  yylval.node_type=new SyntaxTreeNode("COMMA",yytext,line_number);
  return COMMA;
}
{ASSIGN} {
  yylval.node_type=new SyntaxTreeNode("ASSIGN",yytext,line_number);
  return ASSIGN;
}
{LT} {
  yylval.node_type=new SyntaxTreeNode("LT",yytext,line_number);
  return LT;
}
{LE} {
  yylval.node_type=new SyntaxTreeNode("LE",yytext,line_number);
  return LE;
}
{GT} {
  yylval.node_type=new SyntaxTreeNode("GT",yytext,line_number);
  return GT;
}
{GE} {
  yylval.node_type=new SyntaxTreeNode("GE",yytext,line_number);
  return GE;
}
{NE} {
  yylval.node_type=new SyntaxTreeNode("NE",yytext,line_number);
  return NE;
}
{EQ} {
  yylval.node_type=new SyntaxTreeNode("EQ",yytext,line_number);
  return EQ;
}
{PLUS} {
  yylval.node_type=new SyntaxTreeNode("PLUS",yytext,line_number);
  return PLUS;
}
{MINUS} {
  yylval.node_type=new SyntaxTreeNode("MINUS",yytext,line_number);
  return MINUS;
}
{MUL} {
  yylval.node_type=new SyntaxTreeNode("MUL",yytext,line_number);
  return MUL;
}
{DIV} {
  yylval.node_type=new SyntaxTreeNode("DIV",yytext,line_number);
  return DIV;
}
{AND} {
  yylval.node_type=new SyntaxTreeNode("AND",yytext,line_number);
  return AND;
}
{OR} {
  yylval.node_type=new SyntaxTreeNode("OR",yytext,line_number);
  return OR;
}
{NOT} {
  yylval.node_type=new SyntaxTreeNode("NOT",yytext,line_number);
  return NOT;
}
{LP} {
  yylval.node_type=new SyntaxTreeNode("LP",yytext,line_number);
  return LP;
}
{RP} {
  yylval.node_type=new SyntaxTreeNode("RP",yytext,line_number);
  return RP;
}
{LB} {
  yylval.node_type=new SyntaxTreeNode("LB",yytext,line_number);
  return LB;
}
{RB} {
  yylval.node_type=new SyntaxTreeNode("RB",yytext,line_number);
  return RB;
}
{LC} {
  yylval.node_type=new SyntaxTreeNode("LC",yytext,line_number);
  return LC;
}
{RC} {
  yylval.node_type=new SyntaxTreeNode("RC",yytext,line_number);
  return RC;
}
{letter_}({letter_}|{digit})* {
  yylval.node_type=new SyntaxTreeNode("ID",yytext,line_number,TreeNodeType::ID);
  return ID;
}
{digit}({letter_}|{digit})* {
  // invalid id start with number
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ID;
}
\n {
  yycolumn=1;
  line_number += 1;
}
"//" {
    char c = yyinput();
    while(c!='\n'){c=yyinput();}
}
[/][*][^*]*[*]*[*][/]       {
}
 [/][*][^*]*[/][*][^*]*[*]+[/][*]*[*][/]  {  has_error=true;
                                             printf("Error type A at Line %d: nested /*\n", yylineno);
                                             return ERROR;
}
[/][*]                                    {   has_error=true;
                                              printf("Error type A at Line %d: unmateched /*\n", yylineno);
                                              return ERROR; }

[\t\r ]+ {}
. {
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ERROR;
}


%%
