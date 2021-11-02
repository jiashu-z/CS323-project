//
// Created by maozunyao on 2021/10/24.
//
#include "SemanticAnalyzer.h"
extern int customDebug;
void usePrimarySymbol(SyntaxTreeNode *idNode, SymbolTable &symbolTable) {
  Symbol *symbol = symbolTable.searchVariableSymbol(idNode->attribute_value);
  if (symbol == nullptr) {
    std::cout << "Error Type " << 1 << " at Line " << idNode->firstLine << ": "
              << "Undefined variable \"" << idNode->attribute_value << "\"."
              << std::endl;
  }
}
void useFunctionSymbol(SyntaxTreeNode *expNode, SymbolTable &symbolTable) {
  SyntaxTreeNode *idNode = expNode->children[0];
  Symbol *symbol = symbolTable.searchFunctionSymbol(idNode->attribute_value);
  if (symbol == nullptr) {
    std::cout << "Error Type " << 2 << " at Line " << idNode->firstLine << ": "
              << "Undefined function \"" << idNode->attribute_value << "\"."
              << std::endl;
  }
}
void getDecs(SyntaxTreeNode *decList, std::vector<SyntaxTreeNode *> *decs) {
  if (decList->children.size() == 3) {
    decs->push_back(decList->children[0]);
    SyntaxTreeNode *childDecList = decList->children[2];
    getDecs(childDecList, decs);
  } else if (decList->children.size() == 1) {
    decs->push_back(decList->children[0]);
    return;
  } else {
    std::cerr << "fatal error! please check out" << std::endl;
    exit(-1);
  }
}
void insertPrimarySymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable) {
  SyntaxTreeNode *specifier = defNode->children[0];
  SyntaxTreeNode *decList = defNode->children[1];
  std::vector<SyntaxTreeNode *> *decs = new std::vector<SyntaxTreeNode *>();
  getDecs(decList, decs);
  int size = decs->size();
  for (int i = 0; i < size; ++i) {
    SyntaxTreeNode *dec = (*decs)[i];
    std::string symbolName = dec->children[0]->children[0]->attribute_value;
    Symbol *returnPtr = symbolTable.searchVariableSymbol(symbolName);
    if (returnPtr == nullptr) {
      SymbolType symbolType = specifier->expType;
      Symbol *symbol = nullptr;
      switch (symbolType) {
        case SymbolType::INT: {
          IntType *data = new IntType;
          symbol = new Symbol(symbolName, symbolType, data);
          break;
        }
        case SymbolType::FLOAT: {
          FloatType *data = new FloatType;
          symbol = new Symbol(symbolName, symbolType, data);
          break;
        }
        case SymbolType::CHAR: {
          CharType *data = new CharType;
          symbol = new Symbol(symbolName, symbolType, data);
          break;
        }
        default: {
          break;
        }
      }
      if (symbol == nullptr) {
        std::cout << "fatal unhandled error!" << std::endl;
      } else {
        if (customDebug) {
          std::cout << "Symbol " << symbolName << " inserted" << std::endl;
        }
        symbolTable.insertVariableSymbol(symbolName, symbol);
      }
    } else {
      std::cout << "Error Type " << 3 << " at Line "
                << dec->children[0]->children[0]->firstLine << ": "
                << "Redefined variable \""
                << dec->children[0]->children[0]->attribute_value << "\"."
                << std::endl;
    }
  }
}
void insertFunctionSymbolWithoutArgs(SyntaxTreeNode *funDec,
                                     SymbolTable &symbolTable) {
  std::string functionName = funDec->children[0]->attribute_value;
  FunctionType *functionType = new FunctionType;
  Symbol *rt = symbolTable.searchFunctionSymbol(functionName);
  if (rt == nullptr) {
    functionType->functionName = functionName;
    functionType->returnType = funDec->expType;
    Symbol *symbol = new Symbol(functionName, funDec->expType, functionType);
    symbolTable.insertFunctionSymbol(functionName, symbol);
  } else {
    std::cout << "Error type 4 at Line " << funDec->firstLine
              << ": multi definition of function " << functionName << std::endl;
  }
}
void insertFunctionSymbol(SyntaxTreeNode *extDefNode,
                          SymbolTable &symbolTable) {
  if (extDefNode->children.size() == 3) {
    SymbolType functionRtType = extDefNode->children[0]->expType;
    SyntaxTreeNode *funDec = extDefNode->children[1];
    funDec->expType = functionRtType;
    if (funDec->children.size() == 3) {
      insertFunctionSymbolWithoutArgs(funDec, symbolTable);
    } else if (funDec->children.size() == 4) {
      std::cerr << "function with args not supported yet" << std::endl;
    }
  } else {
    std::cout << "fatal unhandled error!" << std::endl;
    exit(-1);
  }
}