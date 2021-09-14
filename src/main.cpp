#include <iostream>
#include <vector>
#include "token.h"

extern std::vector<Token> GetTokens(const std::string& file_path);

int main() {
  int i = 0;
  float f = 1.0;
  std::string file_path = "/home/zhangjiashu/CS323-2021F/project1/src/main.cpp";
  std::vector<Token> tokens = GetTokens(file_path);
  for (auto iter : tokens) {
    std::cout << iter.ToString() << std::endl;
  }
  return 0;
}