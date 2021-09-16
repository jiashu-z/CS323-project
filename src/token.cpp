#include "token.h"
#include <cassert>
#include <string>

const std::string Token::type_names[] = {
  "INT",
  "TYPE", 
  "STRUCT", 
  "IF", 
  "ELSE", 
  "WHILE", 
  "RETURN", 
  "DOT", 
  "SEMI", 
  "COMMA", 
  "ASSIGN", 
  "LT", 
  "LE", 
  "GT", 
  "GE", 
  "NE", 
  "EQ", 
  "PLUS",
  "MINUS",
  "MUL",
  "DIV",
  "AND",
  "OR",
  "NOT",
  "LP",
  "RP",
  "LB",
  "RB",
  "LC",
  "RC",
  "ID",
  };

std::string Token::TypeToString(const Token::Type& token_type) {
  return type_names[token_type];
}

Token::Token(const Token::Type& token_type, const std::string& literal, const int& line_number) : token_type(token_type), 
  literal(literal),
  line_number(line_number) {}

Token::Token(const Token::Type& token_type, const char*& literal, const int& line_number): token_type(token_type), 
  literal(std::string(literal)),
  line_number(line_number) {}

Token::~Token() {}

std::string Token::ToString() const {
  return this->TypeToString(this->token_type) + ": " + this->literal + " @" + std::to_string(this->line_number);
}

Token::Type Token::GetType() const {
  return this->token_type;
}

std::string Token::GetLiteral() const {
  return this->literal;
}
