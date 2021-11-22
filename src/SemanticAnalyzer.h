//
// Created by maozunyao on 2021/10/24.
//

#ifndef CS323_PROJECT1_SEMANTICANALYZER_H
#define CS323_PROJECT1_SEMANTICANALYZER_H
#include <iostream>

#include "SymbolTable.h"
#include "SyntaxTreeNode.h"

void getVariableSymbol(SyntaxTreeNode *idNode, SymbolTable &symbolTable);

void getFunctionSymbol(SyntaxTreeNode *expNode, SymbolTable &symbolTable);

void insertSymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable);

void insertPrimarySymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable);

void insertFunctionSymbol(SyntaxTreeNode *extDefNode, SymbolTable &symbolTable, bool hasDeninition);

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

void checkIfExistFunctionReturnStatement(SyntaxTreeNode *specifier,
                                         SyntaxTreeNode *compSt
);

void assignFunctionReturnType(SyntaxTreeNode *specifier,
                              SyntaxTreeNode *funcDec);

void checkBinaryOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                         SyntaxTreeNode *rightOperand);

void checkDotOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                      SyntaxTreeNode *rightOperand);

void checkAndUpdateExpArray(SyntaxTreeNode *headExpNode,
                            SyntaxTreeNode *arrayIdExpNode,
                            SyntaxTreeNode *indexExpNode);

void updateSyntaxTreeNodeSymbol(SyntaxTreeNode *to, SyntaxTreeNode *from);

void assignStructSpecifierType(SyntaxTreeNode *specifier,
                               SymbolTable &symbolTable);

void assignConstantSymbol(SyntaxTreeNode *node, const SymbolType &symbolType);

void getDefNodes(SyntaxTreeNode *defList,
                 std::vector<SyntaxTreeNode *> *defNodes);

bool checkArrayEquivelence(ArrayType *left, ArrayType *right);

bool checkStructEquivelence(StructType *left, StructType *right);

void checkFunctionArgsType(FunctionType *leftFuntionSymbol, FunctionType *rightFunctionSymbol, int firstLine);

void checkArgsType(std::vector<Symbol *> &expectArgs, std::vector<Symbol *> &actualArgs, int firstLine,
                   std::string functionName);

void insertExtPrimarySymbol(SyntaxTreeNode *extDefNode, SymbolTable &symbolTable);
#endif  // CS323_PROJECT1_SEMANTICANALYZER_H
