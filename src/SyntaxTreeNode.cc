//
// Created by maozunyao on 2021/9/29.
//

#include "SyntaxTreeNode.hh"
#include "token.h"


SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName) {
    this->parent=nullptr;
    this->children = std::vector<SyntaxTreeNode*>();
    this->attribute_name= attributeName;
}
SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName, const std::string &attribute_value) {
    this->parent=nullptr;
    this->children = std::vector<SyntaxTreeNode*>();
    this->attribute_name=attributeName;
    this->attribute_value=attribute_value;
}
SyntaxTreeNode::~SyntaxTreeNode()= default;
std::string SyntaxTreeNode::ToString() const{
    return attribute_name;
}

SyntaxTreeNode *SyntaxTreeNode::getParent() const{
    return parent;
}

const std::vector<SyntaxTreeNode*> & SyntaxTreeNode::getChildren() const {
    return children;
}

const std::string & SyntaxTreeNode::getAttributeName() const{
    return attribute_name;
}
void SyntaxTreeNode::insert(SyntaxTreeNode* newChild){
      this->children.push_back(newChild);
}

void SyntaxTreeNode::preOrderPrint(SyntaxTreeNode *node,int level){

    //std::cout << "level:" << level << std::endl;
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    std::cout << node->attribute_name << std::endl;
    int size = node->children.size();
    for (int i = 0; i < size; i++) {
        SyntaxTreeNode *child = node->children[i];
        preOrderPrint(child, level + 1);
    }

}
void SyntaxTreeNode::insert(std::initializer_list<SyntaxTreeNode*> nodes){

    for(auto beg=nodes.begin(); beg!=nodes.end(); ++beg){
        this->insert(*beg);
    }
}

SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName,int firstLine, int firstColumn){
    this->parent=nullptr;
    this->children = std::vector<SyntaxTreeNode*>();
    this->attribute_name= attributeName;
    this->firstLine=firstLine;
    this->firstColumn=firstColumn;
}

