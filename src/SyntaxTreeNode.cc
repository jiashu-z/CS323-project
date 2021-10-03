//
// Created by maozunyao on 2021/9/29.
//

#include "SyntaxTreeNode.hh"
#include "token.h"
#define DEBUG 0

SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName) {
    this->parent=nullptr;
    this->children = std::vector<SyntaxTreeNode*>();
    this->attribute_name= attributeName;
}
SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName, const std::string &attribute_value,int firstLine) {
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
    if(node== nullptr){
        std::cout<<"Unhandled error, tree node should not be null!"<<std::endl;
        return;
    }
        if(node->nodeType==TreeNodeType::EMPTY){
            return;
        }
    //std::cout << "level:" << level << std::endl;
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    if(DEBUG) {
        std::cout<<node->attribute_name<<std::endl;
//        std::cout << node->attribute_name << ", line: " << node->firstLine << ", text value:" << node->attribute_value
//                  << ", type:" << enumToString((int) node->nodeType) << std::endl;
    }
    else{
        switch (node->nodeType) {

            case TreeNodeType::Non_Terminal:
                std::cout<<node->attribute_name<<" "<<"("<<node->firstLine<<")"<<std::endl;
                break;
            case TreeNodeType::TYPE:
                std::cout<<"TYPE: "<<node->attribute_value<<std::endl;
                break;
            case TreeNodeType::CHAR:
                std::cout<<"CHAR: "<<node->attribute_value<<std::endl;
                break;
            case TreeNodeType::INT: //TODO: hex value not handled
                std::cout<<"INT: "<<node->attribute_value<<std::endl;
                break;
            case TreeNodeType::FLOAT:
                std::cout<<"FLOAT: "<<node->attribute_value<<std::endl;
                break;
            case TreeNodeType::ID:
                std::cout<<"ID: "<<node->attribute_value<<std::endl;
                break;
            case TreeNodeType::OTHER:
                std::cout<<node->attribute_name<<std::endl;
                break;
            case TreeNodeType::EMPTY:
                break;
            default:
                std::cout<<"superrise mother fucker!!!!!!!!"<<std::endl;
        }
    }
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

SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName, const std::string &attributeValue,
                               int firstLine, int firstColumn, TreeNodeType nodeType) : nodeType(nodeType), attribute_name(attributeName),
                                                                                        attribute_value(attributeValue), firstLine(firstLine),
                                                                                        firstColumn(firstColumn) {
    this->parent=nullptr;
    this->children = std::vector<SyntaxTreeNode*>();}

SyntaxTreeNode::SyntaxTreeNode(const std::string &attributeName,
                               const std::string &attributeValue, int firstLine,TreeNodeType nodeType) : nodeType(nodeType),
                                                                                   attribute_name(attributeName),
                                                                                   attribute_value(attributeValue),
                                                                                   firstLine(firstLine) {}

