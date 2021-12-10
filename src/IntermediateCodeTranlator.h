//
// Created by maozunyao on 2021/12/6.
//

#ifndef CS323_PROJECT1_INTERMEDIATECODETRANLATOR_H
#define CS323_PROJECT1_INTERMEDIATECODETRANLATOR_H

#include "IntermediateCode.h"
#include "SyntaxTreeNode.h"


std::vector<IntermediateCode *> &
translate_Args(SyntaxTreeNode *argsNode, SymbolTable &symbolTable, std::vector<std::string> &argList);

std::vector<IntermediateCode *> &translate_Exp(SyntaxTreeNode *exp, SymbolTable &symbolTable, std::string &place);

std::vector<IntermediateCode *> &translate_stmt(SyntaxTreeNode *stmt, SymbolTable &symbolTable);

std::vector<IntermediateCode *> &translate_Compst(SyntaxTreeNode *compst, SymbolTable &symbolTable);

std::vector<IntermediateCode *> &
translate_cond_Exp(SyntaxTreeNode *exp, SymbolTable &symbolTable, std::string &lb_t, std::string lb_f);

std::vector<IntermediateCode *> &translate_fundec(SyntaxTreeNode *fundec, SymbolTable &symbolTable);

std::vector<IntermediateCode *> &translate_fundec_without_args(SyntaxTreeNode *fundec, SymbolTable &symbolTable);

std::vector<IntermediateCode *> &translate_fundec_with_args(SyntaxTreeNode *fundec, SymbolTable &symbolTable);

std::vector<IntermediateCode *> &translate_VarList(SyntaxTreeNode *varList);

std::vector<IntermediateCode *> &translate_dec(SyntaxTreeNode *pNode, SymbolTable &table);

void translateFunctionAndPrint(SyntaxTreeNode *program, SymbolTable &symboltable);

void getStmtNodes(SyntaxTreeNode *stmtList, std::vector<SyntaxTreeNode *> &stmts);

void mergeInterCode(SyntaxTreeNode *merge, std::vector<IntermediateCode *> &);

#endif //CS323_PROJECT1_INTERMEDIATECODETRANLATOR_H
