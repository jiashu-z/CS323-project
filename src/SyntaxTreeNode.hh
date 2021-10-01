//
// Created by maozunyao on 2021/9/29.
//

#ifndef CS323_PROJECT1_SYNTAXTREENODE_HH
#define CS323_PROJECT1_SYNTAXTREENODE_HH

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>

//TODO: node's parent is not used and handled
class SyntaxTreeNode{
    public:
    SyntaxTreeNode * parent{};
    std::vector<SyntaxTreeNode*> children;
    std::string attribute_name;
    std::string attribute_value;
    int firstLine;
    int firstColumn;
    public:
    explicit SyntaxTreeNode(const std::string &attributeName);
    SyntaxTreeNode(const std::string &attributeName,const std::string &attribute_value);

    SyntaxTreeNode(const std::string &attributeName,int firstLine, int firstColumn);
        ~SyntaxTreeNode();
        std::string ToString() const;

    SyntaxTreeNode *getParent() const;

    const std::vector<SyntaxTreeNode*> &getChildren() const;

    const std::string &getAttributeName() const;

    void insert(SyntaxTreeNode *newChild);

    void preOrderPrint(SyntaxTreeNode *node,int level);

    void insert(std::initializer_list<SyntaxTreeNode*> nodes);

};

#endif //CS323_PROJECT1_SYNTAXTREENODE_HH
