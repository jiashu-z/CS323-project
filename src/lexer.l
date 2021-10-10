/*https://stackoverflow.com/questions/15550553/flex-regular-expression-literal-char*/

%{
  int yycolumn = 1;
  #define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng;\
    yycolumn += yyleng;

  #include <iostream>
  #include <vector>
  #include "token.h"
  #include "SyntaxTreeNode.h"
  #include "parser.h"

  std::vector<Token> *tokens_ptr =new std::vector<Token>;
  int line_number = 1;
  bool has_error = false;
  extern "C" int yylex();
%}

%option yylineno

CHAR '.'|'\\x[0-9a-fA-F]{1,2}'
INVALID_CHAR ''|'[^']{2,}'
INVALID_HEX_CHAR '\\x[0-9a-fA-F]*[g-z][0-9a-fA-F]*'|'\\x[0-9a-fA-F]{3,}'
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

%%
{CHAR} {
  yylval.node_type=new SyntaxTreeNode("TYPE",yytext,line_number,TreeNodeType::CHAR);
  return CHAR;
}
{INVALID_CHAR} {
  has_error=true;
  printf("Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
  return ERROR;
}
{INVALID_HEX_CHAR} {
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
  tokens_ptr->push_back(Token(Token::Type::INT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("TYPE",yytext,line_number,TreeNodeType::INT);
  return INT;
}
{HEX_INT} {
  tokens_ptr->push_back(Token(Token::Type::INT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("TYPE",strtol(yytext, NULL, 16),line_number,TreeNodeType::INT);
  return INT;
}
{FLOAT} {
  yylval.node_type=new SyntaxTreeNode("TYPE",yytext,line_number,TreeNodeType::FLOAT);
  return FLOAT;
}
{TYPE} {
  tokens_ptr->push_back(Token(Token::Type::TYPE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("TYPE",yytext,line_number,TreeNodeType::TYPE);
  return TYPE;
}
{STRUCT} {
  tokens_ptr->push_back(Token(Token::Type::STRUCT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("STRUCT",yytext,line_number);
  return STRUCT;
}
{IF} {
  tokens_ptr->push_back(Token(Token::Type::IF, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("IF",yytext,line_number);
  return IF;
}
{ELSE} {
  tokens_ptr->push_back(Token(Token::Type::ELSE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("ELSE",yytext,line_number);
  return ELSE;
}
{WHILE} {
  tokens_ptr->push_back(Token(Token::Type::WHILE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("WHILE",yytext,line_number);
  return WHILE;
}
{FOR} {
  yylval.node_type=new SyntaxTreeNode("WHILE",yytext,line_number);
  return FOR;
}
{RETURN} {
  tokens_ptr->push_back(Token(Token::Type::RETURN, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("RETURN",yytext,line_number);
  return RETURN;
}
{DOT} {
  tokens_ptr->push_back(Token(Token::Type::DOT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("DOT",yytext,line_number);
  return DOT;
}
{SEMI} {
  tokens_ptr->push_back(Token(Token::Type::SEMI, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("SEMI",yytext,line_number);
  return SEMI;
}
{COMMA} {
  tokens_ptr->push_back(Token(Token::Type::COMMA, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("COMMA",yytext,line_number);
  return COMMA;
}
{ASSIGN} {
  tokens_ptr->push_back(Token(Token::Type::ASSIGN, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("ASSIGN",yytext,line_number);
  return ASSIGN;
}
{LT} {
  tokens_ptr->push_back(Token(Token::Type::LT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("LT",yytext,line_number);
  return LT;
}
{LE} {
  tokens_ptr->push_back(Token(Token::Type::LE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("LE",yytext,line_number);
  return LE;
}
{GT} {
  tokens_ptr->push_back(Token(Token::Type::GT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("GT",yytext,line_number);
  return GT;
}
{GE} {
  tokens_ptr->push_back(Token(Token::Type::GE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("GT",yytext,line_number);
  return GE;
}
{NE} {
  tokens_ptr->push_back(Token(Token::Type::NE, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("NE",yytext,line_number);
  return NE;
}
{EQ} {
  tokens_ptr->push_back(Token(Token::Type::EQ, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("EQ",yytext,line_number);
  return EQ;
}
{PLUS} {
  tokens_ptr->push_back(Token(Token::Type::PLUS, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("PLUS",yytext,line_number);
  return PLUS;
}
{MINUS} {
  tokens_ptr->push_back(Token(Token::Type::MINUS, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("MINUS",yytext,line_number);
  return MINUS;
}
{MUL} {
  tokens_ptr->push_back(Token(Token::Type::MUL, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("MUL",yytext,line_number);
  return MUL;
}
{DIV} {
  tokens_ptr->push_back(Token(Token::Type::DIV, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("DIV",yytext,line_number);
  return DIV;
}
{AND} {
  tokens_ptr->push_back(Token(Token::Type::AND, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("AND",yytext,line_number);
  return AND;
}
{OR} {
  tokens_ptr->push_back(Token(Token::Type::OR, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("OR",yytext,line_number);
  return OR;
}
{NOT} {
  tokens_ptr->push_back(Token(Token::Type::NOT, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("NOT",yytext,line_number);
  return NOT;
}
{LP} {
  tokens_ptr->push_back(Token(Token::Type::LP, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("LP",yytext,line_number);
  return LP;
}
{RP} {
  tokens_ptr->push_back(Token(Token::Type::RP, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("RP",yytext,line_number);
  return RP;
}
{LB} {
  tokens_ptr->push_back(Token(Token::Type::LB, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("LB",yytext,line_number);
  return LB;
}
{RB} {
  tokens_ptr->push_back(Token(Token::Type::RB, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("RB",yytext,line_number);
  return RB;
}
{LC} {
  tokens_ptr->push_back(Token(Token::Type::LC, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("LC",yytext,line_number);
  return LC;
}
{RC} {
  tokens_ptr->push_back(Token(Token::Type::RC, yytext, line_number));
  yylval.node_type=new SyntaxTreeNode("RC",yytext,line_number);
  return RC;
}
{letter_}({letter_}|{digit})* {
  tokens_ptr->push_back(Token(Token::Type::ID, yytext, line_number));
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
<<EOF>> {
  yyterminate();
}

%%
std::vector<Token> GetTokens(const std::string& file_path) {
  if (!(yyin = fopen(file_path.c_str(), "r"))) {
    perror(file_path.c_str());
    return *tokens_ptr;
  }
  yylex();
  return *tokens_ptr;
}