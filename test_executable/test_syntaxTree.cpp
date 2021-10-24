//
// Created by maozunyao on 2021/9/30.
//

#include <iostream>
#include "SyntaxTreeNode.h"
#include <cassert>

void test_print();
void test_insert();
int main() {
    int   lTemp   =   strtol("0x10", NULL, 16);
    std::cout<<lTemp<<std::endl;
    char c = '\x55';
    std::cout<<c<<std::endl;
test_print();
test_insert();
    return 0;
}

void test_print(){
    SyntaxTreeNode * root1=new SyntaxTreeNode("Program");
    SyntaxTreeNode * root2=new SyntaxTreeNode("ExtDefList");
    SyntaxTreeNode * root3=new SyntaxTreeNode("ExtDef");
    SyntaxTreeNode * root4=new SyntaxTreeNode("Specifier");
    SyntaxTreeNode * root5=new SyntaxTreeNode("TYPE");
    SyntaxTreeNode * root6=new SyntaxTreeNode("FunDec");
    SyntaxTreeNode * root7=new SyntaxTreeNode("ID");
    SyntaxTreeNode * root8=new SyntaxTreeNode("LP");
    SyntaxTreeNode * root9=new SyntaxTreeNode("VarList");
    SyntaxTreeNode * root10=new SyntaxTreeNode("ParamDec");
    SyntaxTreeNode * root11=new SyntaxTreeNode("Specifier");
    SyntaxTreeNode * root12=new SyntaxTreeNode("TYPE");
    SyntaxTreeNode * root13=new SyntaxTreeNode("VarDec");
    SyntaxTreeNode * root14=new SyntaxTreeNode("ID");
    SyntaxTreeNode * root15=new SyntaxTreeNode("COMMA");
    SyntaxTreeNode * root16=new SyntaxTreeNode("VarList");
    SyntaxTreeNode * root17=new SyntaxTreeNode("ParamDec");
    SyntaxTreeNode * root18=new SyntaxTreeNode("Specifier");
    SyntaxTreeNode * root19=new SyntaxTreeNode("TYPE");
    SyntaxTreeNode * root20=new SyntaxTreeNode("VarDec");
    SyntaxTreeNode * root21=new SyntaxTreeNode("ID");
    SyntaxTreeNode * root22=new SyntaxTreeNode("RP");
    SyntaxTreeNode * root23=new SyntaxTreeNode("CompSt");
    SyntaxTreeNode * root24=new SyntaxTreeNode("LC");
    SyntaxTreeNode * root25=new SyntaxTreeNode("StmtList");
    SyntaxTreeNode * root26=new SyntaxTreeNode("Stmt");
    SyntaxTreeNode * root27=new SyntaxTreeNode("Exp");
    SyntaxTreeNode * root28=new SyntaxTreeNode("Exp");
    SyntaxTreeNode * root29=new SyntaxTreeNode("ID");
    SyntaxTreeNode * root30=new SyntaxTreeNode("ASSIGN");
    SyntaxTreeNode * root31=new SyntaxTreeNode("Exp");
    SyntaxTreeNode * root32=new SyntaxTreeNode("CHAR");
    SyntaxTreeNode * root33=new SyntaxTreeNode("SEMI");
    SyntaxTreeNode * root34=new SyntaxTreeNode("RC");


    root1->insert(root2);
    root2->insert(root3);
    root3->insert(root4);
    root4->insert(root5);
    root3->insert(root6);
    root6->insert(root7);
    root6->insert(root8);
    root6->insert(root9);
    root9->insert(root10);
    root10->insert(root11);
    root11->insert(root12);
    root10->insert(root13);
    root13->insert(root14);
    root9->insert(root15);
    root9->insert(root16);
    root16->insert(root17);
    root17->insert(root18);
    root18->insert(root19);
    root17->insert(root20);
    root20->insert(root21);
    root6->insert(root22);
    root3->insert(root23);
    root23->insert(root24);
    root23->insert(root25);
    root25->insert(root26);
    root26->insert(root27);
    root27->insert(root28);
    root28->insert(root29);
    root27->insert(root30);
    root27->insert(root31);
    root31->insert(root32);
    root26->insert(root33);
    root23->insert(root34);

    root1->preOrderPrint(root1,0);
}
void test_insert(){

    SyntaxTreeNode * root1=new SyntaxTreeNode("Program");
    SyntaxTreeNode * root2=new SyntaxTreeNode("ExtDefList");
    SyntaxTreeNode * root3=new SyntaxTreeNode("ExtDef");
    SyntaxTreeNode * root4=new SyntaxTreeNode("Specifier");
    SyntaxTreeNode * root5=new SyntaxTreeNode("TYPE");
    root1->insert({root2,root3,root4,root5});
    root2->preOrderPrint(root1,0);
}