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

bool SymbolTable::insertFunctionSymbol(std::string name, Symbol *symbol) {
  if (this->currentFunctionTable.find(name) ==
      this->currentFunctionTable.end()) {
    this->currentFunctionTable.emplace(name, symbol);
    return true;
  } else {
    return false;
  }
}

Symbol *SymbolTable::searchFunctionSymbol(std::string name) {
  auto it = this->currentFunctionTable.find(name);
  if (it != currentFunctionTable.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

Symbol::Symbol(const std::string &name, SymbolType symbolType,
               const Symbol::DATA &data)
    : name(name), symbolType(symbolType), symbolData(data) {}

Symbol::Symbol(const std::string &name, SymbolType symbolType) {
    this->name=name;
    this->symbolType=symbolType;
}

ArrayType::ArrayType(Symbol *baseType, int size) : baseType(baseType), size(size){}

ArrayType::ArrayType(Symbol *baseType, int size, int remainDepth) : baseType(baseType), size(size), remainDepth(remainDepth){}