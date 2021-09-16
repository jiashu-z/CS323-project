/*https://stackoverflow.com/questions/15550553/flex-regular-expression-literal-char*/
%{
#define EXIT_OK 0
#define EXIT_FAIL 1

#include <iostream>
#include <vector>
#include "token.h"

std::vector<Token> *tokens_ptr;

extern "C" int yylex();

// global variables
int line_number = 1;

%}
INT ([+-]?[0-9]+)|([+-]?0(x|X)[0-9a-fA-F]+)
TYPE int|float|char
STRUCT struct
IF if
ELSE else
WHILE while
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
{INT} {
  tokens_ptr->push_back(Token(Token::Type::INT, yytext, line_number));
}
{TYPE} {
  tokens_ptr->push_back(Token(Token::Type::TYPE, yytext, line_number));
}
{STRUCT} {
  tokens_ptr->push_back(Token(Token::Type::STRUCT, yytext, line_number));
}
{IF} {
  tokens_ptr->push_back(Token(Token::Type::IF, yytext, line_number));
}
{ELSE} {
  tokens_ptr->push_back(Token(Token::Type::ELSE, yytext, line_number));
}
{WHILE} {
  tokens_ptr->push_back(Token(Token::Type::WHILE, yytext, line_number));
}
{RETURN} {
  tokens_ptr->push_back(Token(Token::Type::RETURN, yytext, line_number));
}
{DOT} {
  tokens_ptr->push_back(Token(Token::Type::DOT, yytext, line_number));
}
{SEMI} {
  tokens_ptr->push_back(Token(Token::Type::SEMI, yytext, line_number));
}
{COMMA} {
  tokens_ptr->push_back(Token(Token::Type::COMMA, yytext, line_number));
}
{ASSIGN} {
  tokens_ptr->push_back(Token(Token::Type::ASSIGN, yytext, line_number));
}
{LT} {
  tokens_ptr->push_back(Token(Token::Type::LT, yytext, line_number));
}
{LE} {
  tokens_ptr->push_back(Token(Token::Type::LE, yytext, line_number));
}
{GT} {
  tokens_ptr->push_back(Token(Token::Type::GT, yytext, line_number));
}
{GE} {
  tokens_ptr->push_back(Token(Token::Type::GE, yytext, line_number));
}
{NE} {
  tokens_ptr->push_back(Token(Token::Type::NE, yytext, line_number));
}
{EQ} {
  tokens_ptr->push_back(Token(Token::Type::EQ, yytext, line_number));
}
{PLUS} {
  tokens_ptr->push_back(Token(Token::Type::PLUS, yytext, line_number));
}
{MINUS} {
  tokens_ptr->push_back(Token(Token::Type::MINUS, yytext, line_number));
}
{MUL} {
  tokens_ptr->push_back(Token(Token::Type::MUL, yytext, line_number));
}
{DIV} {
  tokens_ptr->push_back(Token(Token::Type::DIV, yytext, line_number));
}
{AND} {
  tokens_ptr->push_back(Token(Token::Type::AND, yytext, line_number));
}
{OR} {
  tokens_ptr->push_back(Token(Token::Type::OR, yytext, line_number));
}
{NOT} {
  tokens_ptr->push_back(Token(Token::Type::NOT, yytext, line_number));
}
{LP} {
  tokens_ptr->push_back(Token(Token::Type::LP, yytext, line_number));
}
{RP} {
  tokens_ptr->push_back(Token(Token::Type::RP, yytext, line_number));
}
{LB} {
  tokens_ptr->push_back(Token(Token::Type::LB, yytext, line_number));
}
{RB} {
  tokens_ptr->push_back(Token(Token::Type::RB, yytext, line_number));
}
{LC} {
  tokens_ptr->push_back(Token(Token::Type::LC, yytext, line_number));
}
{RC} {
  tokens_ptr->push_back(Token(Token::Type::RC, yytext, line_number));
}
'([^'\\\n]|\\.)' {}
{letter_}({letter_}|{digit})* {
  tokens_ptr->push_back(Token(Token::Type::ID, yytext, line_number));
} 
\n {
  line_number += 1;
}
[\t\r ]+ { 
  /* does nothing when seeing white spaces except new line */ 
}
. { std::cout << "fallback: " << yytext << std::endl; }
<<EOF>> {
  yyterminate();
}

%%
std::vector<Token> GetTokens(const std::string& file_path) {
  tokens_ptr = new std::vector<Token>();
  if (!(yyin = fopen(file_path.c_str(), "r"))) {
    perror(file_path.c_str());
    return *tokens_ptr;
  }
  yylex();
  return *tokens_ptr;
}