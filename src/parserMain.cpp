//
// Created by maozunyao on 2021/9/30.
//

#include <cstdio>
#include <iostream>

#include "SyntaxTreeNode.h"

void yyrestart(FILE *pFile);

int yyparse(void);

extern SyntaxTreeNode *root_node;
extern int yydebug;
extern int has_error;
extern int error_cnt;
int customDebug = 0;
int main(int argc, char **argv) {
  yydebug = 0;
  if (argc <= 1) {
    std::cout << "PARSER_error_OUTPUT, no input path";
    return 1;
  } else if (argc > 2) {
    std::cout << "too much input path";
    return 1;
  } else {
    FILE *f = fopen(argv[1], "r");
    if (!f) {
      std::cout << "error of path " << argv[1];
      return 1;
    }
    yyrestart(f);
    yyparse();
    if (customDebug) {
      std::cout << "=========Syntax Tree============" << std::endl;
      if (root_node != nullptr && !has_error) {
        root_node->preOrderPrint(root_node, 0);
      }
    }
    if (error_cnt > 0) {
      std::cout << "Syntax error\n";
      error_cnt -= 1;
    }
  }
  return 0;
}
