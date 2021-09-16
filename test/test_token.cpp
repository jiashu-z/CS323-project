#include <iostream>
#include "token.h"
#include <cassert>

void test_ToString() {
  Token token(Token::Type::AND, "&&");
  assert("AND: &&" == token.ToString());
}

void test_GetLiteral() {
  Token token(Token::Type::AND, "&&");
  assert("&&" == token.GetLiteral());
}

int main() {
  test_ToString();
  test_GetLiteral();
  return 0;
}