#include <iostream>
#include "token.h"
#include <cassert>

void test_ToString() {
  Token token(Token::Type::AND, "&&", 0);
  assert("AND: && @0" == token.ToString());
}

void test_GetLiteral() {
  Token token(Token::Type::AND, "&&", 0);
  assert("&&" == token.GetLiteral());
}

int main() {
  test_ToString();
  test_GetLiteral();
  return 0;
}