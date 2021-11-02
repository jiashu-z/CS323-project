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
class SyntaxTreeNode {
 public:
  TreeNodeType nodeType = TreeNodeType::OTHER;
  SyntaxTreeNode *parent{};
  std::vector<SyntaxTreeNode *> children;
  std::string attribute_name;
  std::string attribute_value;
  int firstLine = 0;
  int firstColumn = 0;
  SymbolType expType;  // used for indicate expression data type

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
      case (int)TreeNodeType::FLOAT: {
        return "float";
      }
      case (int)TreeNodeType::CHAR: {
        return "char";
      }
      case (int)TreeNodeType::ID: {
        return "ID";
      }
      case (int)TreeNodeType::OTHER: {
        return "other";
      }
      case (int)TreeNodeType::EMPTY: {
        return "empty";
      }
      default: {
        return "unknown";
      }
    }
  }
};

#endif  // CS323_PROJECT1_SYNTAXTREENODE_H
