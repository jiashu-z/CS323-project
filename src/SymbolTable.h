//
// Created by maozunyao on 2021/10/24.
//

#ifndef CS323_PROJECT1_SYMBOLTABLE_H
#define CS323_PROJECT1_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class SymbolTable;
class Symbol;
enum class SymbolType;
class IntType;
class FloatType;
class CharType;
class ArrayType;
class StructType;
class FunctionType;

enum class SymbolType { INT, FLOAT, CHAR, ARRAY, STRUCT, FUNCTION, UNKNOWN };

class SymbolTable {
 public:
  std::unordered_map<std::string, Symbol *> currentVariableTable;
  std::unordered_map<std::string, Symbol *> currentFunctionDefinitionTable;
  std::unordered_map<std::string, Symbol *> currentFunctionDeclarationTable;
  std::unordered_map<std::string, Symbol *> currentStructDefinitionTable;

  bool insertVariableSymbol(std::string name, Symbol *symbol);

  Symbol *searchVariableSymbol(std::string name);

  bool insertFunctionDefinitionSymbol(std::string name, Symbol *symbol);

  Symbol *searchFunctionDefinitionSymbol(std::string name);

  bool insertFunctionDeclarationSymbol(std::string name, Symbol *symbol);

  Symbol *searchFunctionDeclarationSymbol(std::string name);
};

class Symbol {
 public:
  using DATA = std::variant<IntType *, FloatType *, CharType *, ArrayType *,
                            StructType *, FunctionType *>;
  std::string name;
  SymbolType symbolType = SymbolType::UNKNOWN;
  DATA symbolData;
  Symbol(const std::string &name, const SymbolType &symbolType,
         const DATA &data);
  Symbol(const std::string &name, const SymbolType &symbolType);
};

class ArrayType {
 public:
  Symbol *baseType = nullptr;
  int size = 0;
  int remainDepth = 0;

  ArrayType() = default;
  ArrayType(Symbol *baseType, int size);
  ArrayType(Symbol *baseType, int size, int remainDepth);
};

class IntType {
 public:
  IntType() = default;
};

class FloatType {
 public:
  FloatType() = default;
};

class CharType {
 public:
  CharType() = default;
};
class StructType {
 public:
  std::vector<Symbol *> fieldType;

  StructType() = default;
};

class FunctionType {
 public:
  std::string functionName;
  std::vector<Symbol *> argsType;
  Symbol *returnType;
  bool hasDefinition = true;

  FunctionType() = default;
};

#endif  // CS323_PROJECT1_SYMBOLTABLE_H
