//
// Created by maozunyao on 2021/10/1.
//
#include <initializer_list>  // 头文件
#include <iostream>
#include <string>

std::string func(std::initializer_list<std::string> li) {
  std::string str("");
  for (auto beg = li.begin(); beg != li.end(); ++beg) str += *beg;
  return str;
}

int main() {
  std::cout << func({"This", " ", "is", " ", "C++"}) << std::endl;
  return 0;
}