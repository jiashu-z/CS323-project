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
'([^'\\\n]|\\.)' {}
{letter_}({letter_}|{digit})* { identifiers++; 
printf("line %d: %s\n", lines, yytext); 
tokens_ptr->push_back(Token(Token::Type::identifier, yytext));
} 
\n { lines += 1;  }
[\t\r ]+ { /* does nothing when seeing white spaces except new line */ }
. { std::cout << "fallback: " << yytext << std::endl; }
<<EOF>> { printf("There are %d occurrences of valid identifiers\n", identifiers); yyterminate(); }

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