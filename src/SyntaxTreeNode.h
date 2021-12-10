//
// Created by maozunyao on 2021/9/29.
//

#ifndef CS323_PROJECT1_SYNTAXTREENODE_H
#define CS323_PROJECT1_SYNTAXTREENODE_H

#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

#include "SymbolTable.h"
#include "IntermediateCode.h"

enum class TreeNodeType {
    Non_Terminal = 1,
    TYPE = 2,
    CHAR = 3,
    INT = 4,
    FLOAT = 5,
    ID = 6,
    OTHER = 7,
    EMPTY = 8
};
enum class ProductionEnum {
    EXP_FROM_INT,
    EXP_FROM_ID,
    EXP_FROM_EXP_ASSIGNOP_EXP,
    EXP_FROM_EXP_BINARY_EXP, // +,-,*,/
    EXP_FROM_MINUS_EXP,
    EXP_FROM_EXP_RELOP_EXP,
    EXP_FROM_NOT_EXP,
    EXP_FROM_EXP_AND_EXP,
    EXP_FROM_EXP_OR_EXP,
    EXP_FROM_OTHER_CASES, //copy from textbook
    EXP_FROM_ID_LP_RP,
    EXP_FROM_ID_LP_ARGS_RP,
    EXP_FROM_LP_EXP_RP,
//
    STMT_FROM_EXP_SEMI,
    STMT_FROM_COMPST,
    STMT_FROM_RETURN_EXP_SEMI,
    STMT_IF_LP_EXP_RP_STMT,
    STMT_IF_LP_EXP_RP_STMT_ELSE_STMT,
    STMT_WHILE_LP_EXP_RP_STMT,
//
    ARGS_FROM_EXP,
    ARGS_FROM_EXP_COMMA_ARGS,
    EXTDEF_FROM_FUNC,
    OTHER //do nothing
};

class SyntaxTreeNode {
public:
    TreeNodeType nodeType = TreeNodeType::OTHER;
    SyntaxTreeNode *parent{};
    std::vector<SyntaxTreeNode *> children;
    std::string attribute_name;  //
    std::string attribute_value;
    int firstLine = 0;
    int firstColumn = 0;
    Symbol *symbol = nullptr;
    std::vector<IntermediateCode *> selfAndChildrenCodes;
    ProductionEnum productionEnum = ProductionEnum::OTHER;

public:
    explicit SyntaxTreeNode(const std::string &attributeName);

    SyntaxTreeNode(std::string attributeName, std::string attributeValue,
                   int firstLine, int firstColumn, TreeNodeType nodeType);

    SyntaxTreeNode(std::string attributeName, std::string attributeValue,
                   int firstLine, TreeNodeType nodeType);

    SyntaxTreeNode(const std::string &attributeName, int intAttributeValue,
                   int firstLine, TreeNodeType nodeType);

    SyntaxTreeNode(const std::string &attributeName,
                   const std::string &attribute_value, int firstLine);

    SyntaxTreeNode(const std::string &attributeName, int firstLine,
                   int firstColumn);

  ~SyntaxTreeNode();

  std::string ToString() const;

  SyntaxTreeNode *getParent() const;

  const std::vector<SyntaxTreeNode *> &getChildren() const;

  const std::string &getAttributeName() const;

  void insert(SyntaxTreeNode *newChild);

  void preOrderPrint(SyntaxTreeNode *node, int level);

  void insert(std::initializer_list<SyntaxTreeNode *> nodes);

  std::string enumToString(int enumValue) {
    switch (enumValue) {
      case (int)TreeNodeType::Non_Terminal: {
        return "non-terminal";
      }
      case (int)TreeNodeType::TYPE: {
        return "type";
      }
      case (int)TreeNodeType::INT: {
        return "int";
      }
        case (int) TreeNodeType::FLOAT: {
            return "float";
        }
        case (int) TreeNodeType::CHAR: {
            return "char";
        }
        case (int) TreeNodeType::ID: {
            return "ID";
        }
        case (int) TreeNodeType::OTHER: {
            return "other";
        }
        case (int) TreeNodeType::EMPTY: {
            return "empty";
        }
        default: {
            return "unknown";
        }
    }
  }

    void printInterCode() {
        for (int i = 0; i < this->selfAndChildrenCodes.size(); ++i) {
            IntermediateCode *intermediateCode1 = this->selfAndChildrenCodes.at(i);
            intermediateCode1->print();
        }
    }
};

#endif  // CS323_PROJECT1_SYNTAXTREENODE_H
