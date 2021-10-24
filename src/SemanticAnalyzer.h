//
// Created by maozunyao on 2021/10/24.
//

#ifndef CS323_PROJECT1_SEMANTICANALYZER_H
#define CS323_PROJECT1_SEMANTICANALYZER_H
#include "SyntaxTreeNode.h"
#include "SymbolTable.h"
#include <iostream>
void usePrimarySymbol(SyntaxTreeNode * idNode, SymbolTable &symbolTable);
void insertPrimarySymbol(SyntaxTreeNode* defNode,SymbolTable &symbolTable);
#endif //CS323_PROJECT1_SEMANTICANALYZER_H
