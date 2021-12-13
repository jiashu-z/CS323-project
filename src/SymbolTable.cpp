//
// Created by maozunyao on 2021/10/24.
//

#include "SymbolTable.h"

bool SymbolTable::insertVariableSymbol(std::string name, Symbol *symbol) {
  if (this->currentVariableTable.find(name) ==
      this->currentVariableTable.end()) {
    this->currentVariableTable.emplace(name, symbol);
    return true;
  } else {
    return false;
  }
}

Symbol *SymbolTable::searchVariableSymbol(std::string name) {
  auto it = this->currentVariableTable.find(name);
  if (it != currentVariableTable.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

bool SymbolTable::insertFunctionDefinitionSymbol(std::string name,
                                                 Symbol *symbol) {
  if (this->currentFunctionDefinitionTable.find(name) ==
      this->currentFunctionDefinitionTable.end()) {
    this->currentFunctionDefinitionTable.emplace(name, symbol);
    return true;
  } else {
    return false;
  }
}

bool SymbolTable::insertFunctionDeclarationSymbol(std::string name,
                                                  Symbol *symbol) {
  if (this->currentFunctionDeclarationTable.find(name) ==
      this->currentFunctionDeclarationTable.end()) {
    this->currentFunctionDeclarationTable.emplace(name, symbol);
    return true;
  } else {
    return false;
  }
}

Symbol *SymbolTable::searchFunctionDefinitionSymbol(std::string name) {
  auto it = this->currentFunctionDefinitionTable.find(name);
  if (it != currentFunctionDefinitionTable.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

Symbol *SymbolTable::searchFunctionDeclarationSymbol(std::string name) {
  auto it = this->currentFunctionDeclarationTable.find(name);
  if (it != currentFunctionDeclarationTable.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

Symbol::Symbol(const std::string &name, const SymbolType &symbolType,
               const Symbol::DATA &data)
    : name(name), symbolType(symbolType), symbolData(data) {}

Symbol::Symbol(const std::string &name, const SymbolType &symbolType) {
  this->name = name;
  this->symbolType = symbolType;
}

ArrayType::ArrayType(Symbol *baseType, int size)
    : baseType(baseType), size(size) {}

ArrayType::ArrayType(Symbol *baseType, int size, int remainDepth)
    : baseType(baseType), size(size), remainDepth(remainDepth) {}