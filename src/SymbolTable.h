//
// Created by maozunyao on 2021/10/24.
//

#ifndef CS323_PROJECT1_SYMBOLTABLE_H
#define CS323_PROJECT1_SYMBOLTABLE_H

#include <vector>
#include <variant>
#include <string>
#include <unordered_map>

class SymbolTable;
class Symbol;
class IntType;
class FloatType;
class CharType;
class ArrayType;
class StructType;
class FunctionType;

enum class SymbolType {
    INT,
    FLOAT,
    CHAR,
    ARRAY,
    STRUCT,
    FUNCTION,
    NONE
};

class SymbolTable {
public:
    std::unordered_map<std::string,Symbol*> currentTable;
    bool insertSymbol(std::string name,Symbol * symbol);
    Symbol* searchSymbol(std::string name);
};

class Symbol{
public:
    using DATA = std::variant<IntType*,FloatType*,CharType*,ArrayType*,StructType*,FunctionType*>;
    std::string name;
    SymbolType symbolType = SymbolType::INT;
    DATA data;

    Symbol(const std::string &name, SymbolType symbolType, const DATA &data);
};



class ArrayType{
public:
    Symbol* baseType=nullptr;
    int size=0;

    ArrayType();

};

class IntType {
public:
    IntType();
};

class FloatType{
public:
    FloatType();
};

class CharType{
public:
    CharType();
};
class StructType{
public:
    std::vector<std::string> fieldName;
    std::vector<Symbol> fieldType;

    StructType();
};

class FunctionType{
public:
    std::string functionName;
    std::vector<std::string> argsName;
    std::vector<Symbol> argsType;
    SymbolType returnType;

    FunctionType();
};





#endif //CS323_PROJECT1_SYMBOLTABLE_H
