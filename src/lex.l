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
int identifiers = 0;
int lines = 0;
%}
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
{TYPE} {
  tokens_ptr->push_back(Token(Token::Type::TYPE, yytext));
}
{STRUCT} {
  tokens_ptr->push_back(Token(Token::Type::STRUCT, yytext));
}
{IF} {
  tokens_ptr->push_back(Token(Token::Type::IF, yytext));
}
{ELSE} {
  tokens_ptr->push_back(Token(Token::Type::ELSE, yytext));
}
{WHILE} {
  tokens_ptr->push_back(Token(Token::Type::WHILE, yytext));
}
{RETURN} {
  tokens_ptr->push_back(Token(Token::Type::RETURN, yytext));
}
{DOT} {
  tokens_ptr->push_back(Token(Token::Type::DOT, yytext));
}
{SEMI} {
  tokens_ptr->push_back(Token(Token::Type::SEMI, yytext));
}
{COMMA} {
  tokens_ptr->push_back(Token(Token::Type::COMMA, yytext));
}
{ASSIGN} {
  tokens_ptr->push_back(Token(Token::Type::ASSIGN, yytext));
}
{LT} {
  tokens_ptr->push_back(Token(Token::Type::LT, yytext));
}
{LE} {
  tokens_ptr->push_back(Token(Token::Type::LE, yytext));
}
{GT} {
  tokens_ptr->push_back(Token(Token::Type::GT, yytext));
}
{GE} {
  tokens_ptr->push_back(Token(Token::Type::GE, yytext));
}
{NE} {
  tokens_ptr->push_back(Token(Token::Type::NE, yytext));
}
{EQ} {
  tokens_ptr->push_back(Token(Token::Type::EQ, yytext));
}
{PLUS} {
  tokens_ptr->push_back(Token(Token::Type::PLUS, yytext));
}
{MINUS} {
  tokens_ptr->push_back(Token(Token::Type::MINUS, yytext));
}
{MUL} {
  tokens_ptr->push_back(Token(Token::Type::MUL, yytext));
}
{DIV} {
  tokens_ptr->push_back(Token(Token::Type::DIV, yytext));
}
{AND} {
  tokens_ptr->push_back(Token(Token::Type::AND, yytext));
}
{OR} {
  tokens_ptr->push_back(Token(Token::Type::OR, yytext));
}
{NOT} {
  tokens_ptr->push_back(Token(Token::Type::NOT, yytext));
}
{LP} {
  tokens_ptr->push_back(Token(Token::Type::LP, yytext));
}
{RP} {
  tokens_ptr->push_back(Token(Token::Type::RP, yytext));
}
{LB} {
  tokens_ptr->push_back(Token(Token::Type::LB, yytext));
}
{RB} {
  tokens_ptr->push_back(Token(Token::Type::RB, yytext));
}
{LC} {
  tokens_ptr->push_back(Token(Token::Type::LC, yytext));
}
{RC} {
  tokens_ptr->push_back(Token(Token::Type::RC, yytext));
}
'([^'\\\n]|\\.)' {}
{letter_}({letter_}|{digit})* {
  identifiers++;
  tokens_ptr->push_back(Token(Token::Type::ID, yytext));
} 
\n {
  lines += 1;
}
[\t\r ]+ { 
  /* does nothing when seeing white spaces except new line */ 
}
. { std::cout << "fallback: " << yytext << std::endl; }
<<EOF>> {
  printf("There are %d occurrences of valid identifiers\n", identifiers);
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