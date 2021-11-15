//
// Created by maozunyao on 2021/10/24.
//

#ifndef CS323_PROJECT1_SEMANTICANALYZER_H
#define CS323_PROJECT1_SEMANTICANALYZER_H
#include <iostream>

#include "SymbolTable.h"
#include "SyntaxTreeNode.h"

void usePrimarySymbol(SyntaxTreeNode *idNode, SymbolTable &symbolTable);

void useFunctionSymbol(SyntaxTreeNode *expNode, SymbolTable &symbolTable);

void insertSymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable);

void insertPrimarySymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable);

void insertFunctionSymbol(SyntaxTreeNode *extDefNode, SymbolTable &symbolTable);

void insertStructDefinitionSymbol(SyntaxTreeNode *defNode,
                                  SymbolTable &symbolTable);

void insertStructSymbol(SyntaxTreeNode *extDefNode, SymbolTable &symbolTable);

void checkAssignDataType(SyntaxTreeNode *left, SyntaxTreeNode *right);

void assignIDArrayType(SyntaxTreeNode *parent, SyntaxTreeNode *varDec);

void assignVarDecIDType(SyntaxTreeNode *varDec, SyntaxTreeNode *exp);

void assignSpecifierType(SyntaxTreeNode *specifier);

void checkrValue(SyntaxTreeNode *exp);

void checkReturnType(SyntaxTreeNode *rtExp);

void printErrorMessage(int errorType, int errorLine,
                       const std::string &errorMessage);

void checkFunctionReturnStatement(SyntaxTreeNode *specifier,
                                  SyntaxTreeNode *CompSt);

void assignFunctionReturnType(SyntaxTreeNode *specifier,
                              SyntaxTreeNode *funcDec,
                              SymbolTable &symbolTable);

void checkBinaryOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                         SyntaxTreeNode *rightOperand);

void checkDotOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                      SyntaxTreeNode *rightOperand, SymbolTable &symbolTable);

void checkAndUpdateExpArray(SyntaxTreeNode *headExpNode,
                            SyntaxTreeNode *arrayIdExpNode,
                            SyntaxTreeNode *indexExpNode,
                            SymbolTable &symbolTable);

void updateSyntaxTreeNodeSymbol(SyntaxTreeNode *to, SyntaxTreeNode *from);

void assignStructSpecifierType(SyntaxTreeNode *specifier,
                               SymbolTable &symbolTable);

void assignConstantSymbol(SyntaxTreeNode *node, const SymbolType &symbolType);

void getDefNodes(SyntaxTreeNode *defList,
                 std::vector<SyntaxTreeNode *> *defNodes);
#endif  // CS323_PROJECT1_SEMANTICANALYZER_H
