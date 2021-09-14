#include "token.h"

const std::string Token::type_names[] = {"TYPE", 
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
"reserved", 
"identifier", 
"op"};

std::string Token::TypeToString(const Token::Type& token_type) {
  return type_names[token_type];
}

Token::Token(const Token::Type& token_type, const std::string& literal) : token_type(token_type), literal(literal) {}

Token::Token(const Token::Type& token_type, const char*& literal): token_type(token_type), literal(std::string(literal)) {}

Token::~Token() {}

std::string Token::ToString() const {
  return "{type :" + this->TypeToString(this->token_type) + ", literal: " + this->literal + "}";
}

Token::Type Token::GetType() const {
  return this->token_type;
}

std::string Token::GetLiteral() const {
  return this->literal;
}
