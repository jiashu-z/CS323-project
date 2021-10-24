#include <iostream>
#include <vector>
#include "token.h"
extern std::vector<Token> GetTokens(const std::string& file_path);

int main() {
  int i = 0;
  float f = 1.0;
  std::string file_path =
      "/home/maozunyao/CLionProjects/CS323-project1/demo_test/test_1_r01.spl";
  std::vector<Token> tokens = GetTokens(file_path);
  for (auto iter : tokens) {
    std::cout << iter.ToString() << std::endl;
  }
  return 0;
}