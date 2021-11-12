//
// Created by maozunyao on 2021/10/24.
//
#include "SemanticAnalyzer.h"
#include <exception>
#include <unordered_map>
#include <utility>

extern int customDebug;

void printErrorMessage(int errorType, int errorLine,
                       const std::string &errorMessage) {
  std::cout << "Error type " << errorType << " at Line " << errorLine << ": "
            << errorMessage << std::endl;
}

void checkBinaryOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                         SyntaxTreeNode *rightOperand) {
  bool number = (leftOperand->expType == SymbolType::INT ||
                 leftOperand->expType == SymbolType::FLOAT) &&
                (rightOperand->expType == SymbolType::INT ||
                 rightOperand->expType == SymbolType::FLOAT);
  bool typeEqual = (leftOperand->expType == rightOperand->expType);
  if (!(number && typeEqual)) {
    std::string errorMessage = "binary operation on non-number variables";
    parentExp->expType = SymbolType::UNKNOWN;
    printErrorMessage(7, leftOperand->firstLine, errorMessage);
  }
}

void usePrimarySymbol(SyntaxTreeNode *idNode, SymbolTable &symbolTable) {
  std::string name = idNode->attribute_value;
  Symbol *symbol = symbolTable.searchVariableSymbol(name);
  if (symbol == nullptr) {
    std::string errorMessage = "undefined variable: " + name;
    printErrorMessage(1, idNode->firstLine, errorMessage);
  } else {
    switch (symbol->symbolType) {
      case SymbolType::INT: {
        idNode->expType = SymbolType::INT;
        break;
      }
      case SymbolType::FLOAT: {
        idNode->expType = SymbolType::FLOAT;
        break;
      }
      case SymbolType::CHAR: {
        idNode->expType = SymbolType::CHAR;
        break;
      }
      case SymbolType::STRUCT: {
        idNode->expType = SymbolType::STRUCT;
        idNode->symbol =
                symbolTable.searchVariableSymbol(name);
        break;
      }
      case SymbolType::ARRAY: {
        idNode->expType = SymbolType::ARRAY;
        break;
      }
      default: {
        idNode->expType = SymbolType::UNKNOWN;
        break;
      }
    }
  }
}

int getArgsNum(SyntaxTreeNode *args) {
  SyntaxTreeNode *currentArgs = args;
  int argNum = 0;
  while (true) {
    if (currentArgs->getChildren().size() == 3) {
      argNum++;
      currentArgs = currentArgs->getChildren()[2];
    } else {
      argNum++;
      break;
    }
  }
  return argNum;
}

void useFunctionSymbol(SyntaxTreeNode *expNode, SymbolTable &symbolTable) {
  SyntaxTreeNode *idNode = expNode->children[0];
  Symbol *functionSymbol =
          symbolTable.searchFunctionSymbol(idNode->attribute_value);
  if (functionSymbol == nullptr) {
    Symbol *symbol2 = symbolTable.searchVariableSymbol(idNode->attribute_value);
    if (symbol2 != nullptr) {
      std::string errorMessage =
              "invoking non-function variable: " + idNode->attribute_value;
      printErrorMessage(11, idNode->firstLine, errorMessage);
    } else {
      std::string errorMessage =
              "undefined function: " + idNode->attribute_value;
      printErrorMessage(2, idNode->firstLine, errorMessage);
    }
  } else {
    FunctionType *functionType =
            std::get<FunctionType *>(functionSymbol->symbolData);
    idNode->expType = functionType->returnType;
    int argsExpectNum = functionType->argsType.size();
    int argsActualNum = 0;
    if (expNode->children.size() == 3) {
      if (argsExpectNum != argsActualNum) {
        std::string errorMessage = "invalid argument number for " +
                                   idNode->attribute_value + ", expect " +
                                   std::to_string(argsExpectNum) + ", got " +
                                   std::to_string(argsActualNum);
        printErrorMessage(9, expNode->firstLine, errorMessage);
      }
    } else if (expNode->children.size() == 4) {
      argsActualNum = getArgsNum(expNode->children[2]);
      if (argsExpectNum != argsActualNum) {
        std::string errorMessage = "invalid argument number for " +
                                   idNode->attribute_value + ", expect " +
                                   std::to_string(argsExpectNum) + ", got " +
                                   std::to_string(argsActualNum);
        printErrorMessage(9, expNode->firstLine, errorMessage);
      }
    }
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
    exit(0);
  }
}

bool isArrayType(SyntaxTreeNode *node) {
  if (node->getChildren().empty()) {
    return false;
  }

  while (!node->getChildren().empty()) {
    node = node->children[0];
  }
  if (node->expType == SymbolType::ARRAY) {
    return true;
  } else {
    return false;
  }
}

Symbol *getOneDimArrBaseSymbol(SyntaxTreeNode *specifier, SymbolTable &symbolTable) {
  SymbolType baseType = specifier->expType;
  Symbol *baseSymbol = nullptr;
  switch (baseType) {
    case SymbolType::INT: {
      baseSymbol = new Symbol("", baseType, new IntType);
      break;
    }
    case SymbolType::FLOAT: {
      baseSymbol = new Symbol("", baseType, new FloatType);
      break;
    }
    case SymbolType::CHAR: {
      baseSymbol = new Symbol("", baseType, new CharType);
      break;
    }
    case SymbolType::STRUCT: {
      const std::string &structTypeName =
              specifier->children[0]->children[1]->attribute_value;
      baseSymbol = symbolTable.currentStructDefinitionTable[structTypeName];
      break;
    }
    default: {
      break;
    }
  }
  return baseSymbol;
}

Symbol *getMultiDimArrBaseSymbol(SyntaxTreeNode *node, SyntaxTreeNode *specifier,
                                 const std::string &symbolName, SymbolTable &symbolTable,
                                 int &depth, std::vector<int> &sizes) {
  if (node->getChildren().size() > 1) {
    int size = stoi(node->parent->children[2]->attribute_value);
    sizes.push_back(size);

    Symbol *baseSymbol = getMultiDimArrBaseSymbol(node->children[0], specifier, symbolName,
                                                  symbolTable, depth, sizes);
    depth += 1;
    auto *data = new ArrayType(baseSymbol, 0, depth);
    auto *symbol = new Symbol(symbolName, SymbolType::ARRAY, data);
    return symbol;
  } else {
    depth = 0;
    sizes.push_back(stoi(node->parent->children[2]->attribute_value));

    Symbol *baseSymbol = getOneDimArrBaseSymbol(specifier, symbolTable);
    auto *data = new ArrayType(baseSymbol, 0, depth);
    auto *symbol = new Symbol(symbolName, SymbolType::ARRAY, data);
    return symbol;
  }
}

void changeMultiDimArrSize(Symbol *symbol, std::vector<int> &sizes) {
  ArrayType *data = std::get<ArrayType *>(symbol->symbolData);
  if (data->baseType->symbolType == SymbolType::ARRAY) {
    changeMultiDimArrSize(data->baseType, sizes);
    data->size = sizes.front();
    sizes.erase(sizes.begin());
  } else {
    return;
  }
}

Symbol *getArrSymbol(SyntaxTreeNode *node, SyntaxTreeNode *specifier,
                     const std::string &symbolName, SymbolTable &symbolTable) {
  Symbol *baseSymbol;
  int depth = -1;
  std::vector<int> sizes;
  if (node->getChildren().size() > 1) {
    // multi-dimensional array
    baseSymbol = getMultiDimArrBaseSymbol(node, specifier, symbolName, symbolTable, depth, sizes);
  } else {
    baseSymbol = getOneDimArrBaseSymbol(specifier, symbolTable);
  }
  auto *data = new ArrayType(baseSymbol, stoi(node->parent->children[2]->attribute_value), depth + 1);
  auto *symbol = new Symbol(symbolName, SymbolType::ARRAY, data);
  changeMultiDimArrSize(symbol, sizes);
  return symbol;
}

void insertPrimarySymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable) {
  SyntaxTreeNode *specifier = defNode->children[0];
  SyntaxTreeNode *decList = defNode->children[1];

  auto *decs = new std::vector<SyntaxTreeNode *>();
  getDecs(decList, decs);
  for (auto &dec: *decs) {
    SyntaxTreeNode *node = dec->children[0]->children[0];
    std::string symbolName = node->attribute_value;

    Symbol *returnPtr = symbolTable.searchVariableSymbol(symbolName);
    if (returnPtr == nullptr) {
      SymbolType symbolType = specifier->expType;
      if (isArrayType(node)) {
        symbolType = SymbolType::ARRAY;
      }

      Symbol *symbol = nullptr;
      switch (symbolType) {
        case SymbolType::INT: {
          IntType *data = new IntType;
          symbol = new Symbol(symbolName, symbolType, data);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        case SymbolType::FLOAT: {
          FloatType *data = new FloatType;
          symbol = new Symbol(symbolName, symbolType, data);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        case SymbolType::CHAR: {
          CharType *data = new CharType;
          symbol = new Symbol(symbolName, symbolType, data);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        case SymbolType::STRUCT: {
          const std::string &structTypeName =
                  specifier->children[0]->children[1]->attribute_value;
          auto &structDefinitionTable =
                  symbolTable.currentStructDefinitionTable;
          if (structDefinitionTable.find(structTypeName) ==
              structDefinitionTable.end()) {
            printErrorMessage(16, defNode->firstLine,
                              "Undefined struct error.");
          } else {
            symbol = structDefinitionTable[structTypeName];
            symbolTable.insertVariableSymbol(symbolName, symbol);
            defNode->symbol = symbol;
          }
          break;
        }
        case SymbolType::ARRAY: {
          symbol = getArrSymbol(node, specifier, symbolName, symbolTable);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        default: {
          break;
        }
      }
    } else {
      std::string errorMessage = "redefine variable: " + symbolName;
      printErrorMessage(3, node->firstLine, errorMessage);
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
    Symbol *symbol = new Symbol(functionName, funDec->expType, functionType);
    symbolTable.insertFunctionSymbol(functionName, symbol);
  } else {
    std::string errorMessage = "redefine function: " + functionName;
    printErrorMessage(4, funDec->firstLine, errorMessage);
  }
}

void insertVarListToFunctionType(FunctionType *functionType,
                                 SyntaxTreeNode *varList,
                                 SymbolTable &symbolTable) {
  SyntaxTreeNode *paraDec = varList->children[0];
  SyntaxTreeNode *specifier = paraDec->children[0];
  SyntaxTreeNode *id = paraDec->children[1]->children[0];
  std::string varName = id->attribute_value;
  SymbolType expType = specifier->expType;
  if (isArrayType(id)) {
    expType = SymbolType::ARRAY;
  }

  bool rt = true;
  switch (expType) {
    case SymbolType::INT: {
      IntType *intType = new IntType;
      Symbol *symbol = new Symbol(varName, expType, intType);
      functionType->argsType.push_back(symbol);
      rt = symbolTable.insertVariableSymbol(varName, symbol);
      break;
    }
    case SymbolType::FLOAT: {
      FloatType *floatType = new FloatType;
      Symbol *symbol = new Symbol(varName, expType, floatType);
      functionType->argsType.push_back(symbol);
      rt = symbolTable.insertVariableSymbol(varName, symbol);

      break;
    }
    case SymbolType::CHAR: {
      CharType *charType = new CharType;
      Symbol *symbol = new Symbol(varName, expType, charType);
      functionType->argsType.push_back(symbol);
      rt = symbolTable.insertVariableSymbol(varName, symbol);
      break;
    }
    case SymbolType::ARRAY: {
      Symbol *symbol = getArrSymbol(id, specifier, varName, symbolTable);
      functionType->argsType.push_back(symbol);
      rt = symbolTable.insertVariableSymbol(varName, symbol);
      break;
    }
    default: {
      std::cout << "struct type are not supported yet > <"
                << std::endl;
    }
  }
  if (!rt) {
    std::string errorMessage = "redefine variable: " + id->attribute_value;
    printErrorMessage(3, id->firstLine, errorMessage);
  } else {
    if (customDebug) {
      std::cout << varName << " is inserted" << std::endl;
    }
  }

  if (varList->children.size() == 3) {
    SyntaxTreeNode *childVarList = varList->getChildren()[2];
    insertVarListToFunctionType(functionType, childVarList, symbolTable);
  }
}

void insertFunctionSymbolWithArgs(SyntaxTreeNode *funDec,
                                  SymbolTable &symbolTable) {
  std::string functionName = funDec->children[0]->attribute_value;
  FunctionType *functionType = new FunctionType;
  Symbol *rt = symbolTable.searchFunctionSymbol(functionName);
  SyntaxTreeNode *varList = funDec->children[2];
  if (rt == nullptr) {
    functionType->functionName = functionName;
    insertVarListToFunctionType(functionType, varList, symbolTable);
    Symbol *symbol = new Symbol(functionName, funDec->expType, functionType);
    symbolTable.insertFunctionSymbol(functionName, symbol);
  } else {
    std::string errorMessage = "redefine function: " + functionName;
    printErrorMessage(4, funDec->firstLine, errorMessage);
    exit(0);
  }
}

void insertFunctionSymbol(SyntaxTreeNode *funDec, SymbolTable &symbolTable) {
  if (funDec->children.size() == 3) {
    insertFunctionSymbolWithoutArgs(funDec, symbolTable);
  } else if (funDec->children.size() == 4) {
    insertFunctionSymbolWithArgs(funDec, symbolTable);
  }
}

void assignIDArrayType(SyntaxTreeNode *varDec) {
  if (varDec->getChildren().size() == 1) {
    varDec->children[0]->expType = SymbolType::ARRAY;
  }
  varDec->attribute_value = varDec->children[0]->attribute_value;
}

void assignVarDecIDType(SyntaxTreeNode *varDec, SyntaxTreeNode *exp) {
  if (varDec->getChildren().size() == 1) {
    varDec->children[0]->expType = exp->expType;
  }
}

void checkAssignDataType(SyntaxTreeNode *left, SyntaxTreeNode *right) {
  if (left->expType != right->expType ||
      right->expType == SymbolType::UNKNOWN) {
    std::string errorMessage = "unmatching type on both sides of assignment";
    printErrorMessage(5, left->firstLine, errorMessage);
  }
}

void insertStructDefinitionSymbol(SyntaxTreeNode *structSpecifierNode,
                                  SymbolTable &symbolTable) {
  auto &structDefinitionTable = symbolTable.currentStructDefinitionTable;
  std::string typeName = structSpecifierNode->children[1]->attribute_value;

  if (structDefinitionTable.find(typeName) == structDefinitionTable.end()) {
    std::string &name = typeName;
    SymbolType expType = SymbolType::STRUCT;
    SymbolType &symbolType = expType;
    StructType *data = new StructType;
    SyntaxTreeNode *defListNode = structSpecifierNode->children[3];

    while (!defListNode->children.empty()) {
      SyntaxTreeNode *defNode = defListNode->children[0];
      SyntaxTreeNode *specifierNode = defNode->children[0];
      SyntaxTreeNode *decListNode = defNode->children[1];

      while (true) {
        SyntaxTreeNode *decNode = decListNode->children[0];
        SyntaxTreeNode *varDecNode = decNode->children[0];

        while (varDecNode->children.size() != 1) {
          varDecNode = varDecNode->children[0];
        }
        SyntaxTreeNode *idNode = varDecNode->children[0];
        std::string &varName = idNode->attribute_value;

        Symbol *symbol;
        switch (specifierNode->expType) {
          case SymbolType::INT: {
            symbol = new Symbol(varName, specifierNode->expType, new IntType);
            break;
          }
          case SymbolType::FLOAT: {
            symbol = new Symbol(varName, specifierNode->expType, new FloatType);
            break;
          }
          case SymbolType::CHAR: {
            symbol = new Symbol(varName, specifierNode->expType, new CharType);
            break;
          }
          case SymbolType::ARRAY: {
            // TODO: handled in insertPrimarySymbol
            break;
          }
          case SymbolType::STRUCT: {
            std::string &typeName =
                    specifierNode->children[0]->children[1]->attribute_value;
            if (structDefinitionTable.find(typeName) !=
                structDefinitionTable.end()) {
              symbol = structDefinitionTable[typeName];
            } else {
              printErrorMessage(-1, -1, "todo");
            }
            break;
          }
          default: {
            printErrorMessage(-1, -1, "todo");
          }
        }

        data->fieldName.push_back(varName);
        data->fieldType.push_back(symbol);

        if (decListNode->children.size() > 1) {
          decListNode = decListNode->children[2];
        } else {
          break;
        }
      }
      SymbolType &expType = specifierNode->expType;

      defListNode = defListNode->children[1];
    }
    Symbol *symbol = new Symbol(name, symbolType, data);
    structDefinitionTable.insert(std::make_pair(typeName, symbol));
  } else {
    printErrorMessage(15, structSpecifierNode->firstLine,
                      "redefine struct: " +
                      structSpecifierNode->children[1]->attribute_value);
  }
}

void insertStructSymbol(SyntaxTreeNode *extDefNode, SymbolTable &symbolTable) {
  /*
   * handle definition below
   * struct A{} a1, a2[1], a3[1][1];
   */
  SyntaxTreeNode *specifier = extDefNode->children[0];
  SyntaxTreeNode *extDecList = extDefNode->children[1];

  auto *extDecs = new std::vector<SyntaxTreeNode *>();
  getDecs(extDecList, extDecs);
  for (auto &extDec: *extDecs) {
    SyntaxTreeNode *node = extDec->children[0];
    std::string symbolName = node->attribute_value;

    Symbol *returnPtr = symbolTable.searchVariableSymbol(symbolName);
    if (returnPtr == nullptr) {
      SymbolType symbolType = SymbolType::STRUCT;
      if (isArrayType(node)) {
        symbolType = SymbolType::ARRAY;
      }

      Symbol *symbol = nullptr;
      switch (symbolType) {
        case SymbolType::STRUCT: {
          const std::string &structTypeName =
                  specifier->children[0]->children[1]->attribute_value;
          auto &structDefinitionTable =
                  symbolTable.currentStructDefinitionTable;
          if (structDefinitionTable.find(structTypeName) ==
              structDefinitionTable.end()) {
            printErrorMessage(16, extDefNode->firstLine,
                              "Undefined struct error.");
          } else {
            symbol = structDefinitionTable[structTypeName];
            symbolTable.insertVariableSymbol(symbolName, symbol);
            extDefNode->symbol = symbol;
          }
          break;
        }
        case SymbolType::ARRAY: {
          symbol = getArrSymbol(node, specifier, symbolName, symbolTable);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        default: {
          break;
        }
      }
    } else {
      std::string errorMessage = "redefine variable: " + symbolName;
      printErrorMessage(3, node->firstLine, errorMessage);
    }
  }
}

void assignSpecifierType(SyntaxTreeNode *specifier) {
  SyntaxTreeNode *type = specifier->getChildren()[0];
  if (type->attribute_value == "int") {
    specifier->expType = SymbolType::INT;
  } else if (type->attribute_value == "float") {
    specifier->expType = SymbolType::FLOAT;
  } else if (type->attribute_value == "char") {
    specifier->expType = SymbolType::CHAR;
  } else {
    std::cout << "fatal error!" << std::endl;
    exit(0);
  }
}

void checkrValue(SyntaxTreeNode *exp) {
  if (exp->children.size() == 1) {
    SyntaxTreeNode *child = exp->children[0];
    if (child->nodeType == TreeNodeType::INT ||
        child->nodeType == TreeNodeType::FLOAT ||
        child->nodeType == TreeNodeType::CHAR) {
      std::string errorMessage = "left side in assignment is rvalue";
      printErrorMessage(6, exp->firstLine, errorMessage);
    }
  }
}

void checkReturnType(SyntaxTreeNode *exp) {
  SymbolType rtType = exp->expType;
  SyntaxTreeNode *parent = exp->parent;
  while (parent->attribute_name != "ExtDef") {
    parent = parent->parent;
  }
  SymbolType specifierType = parent->getChildren()[0]->expType;
  if (specifierType != rtType) {
    std::string errorMessage = "incompatiable return type";
    printErrorMessage(8, exp->firstLine, errorMessage);
    exit(0);
  }
}

void preOrderCheckStmt(SyntaxTreeNode *node, SymbolType &rtType) {
  if (node->children.empty()) {
    return;
  } else if (node->children[0]->attribute_name == "RETURN") {
    SyntaxTreeNode *exp = node->children[1];
    if (exp->expType != rtType) {
      std::string errorMesaage = "incompatiable return type";
      printErrorMessage(8, exp->firstLine, errorMesaage);
    }
  }
  for (auto &i: node->children) {
    preOrderCheckStmt(i, rtType);
  }
}

void checkFunctionReturnStatement(SyntaxTreeNode *specifier,
                                  SyntaxTreeNode *compSt) {
  SymbolType rtType = specifier->expType;
  SyntaxTreeNode *stmtList = compSt->children[2];
  preOrderCheckStmt(stmtList, rtType);
}

void assignFunctionReturnType(SyntaxTreeNode *specifier,
                              SyntaxTreeNode *funcDec,
                              SymbolTable &symbolTable) {
  std::string functionName = funcDec->children[0]->attribute_value;
  Symbol *functionSymbol = symbolTable.searchFunctionSymbol(functionName);
  FunctionType *functionType =
          std::get<FunctionType *>(functionSymbol->symbolData);
  functionType->returnType = specifier->expType;
}

void checkDotOperator(SyntaxTreeNode *parentExp, SyntaxTreeNode *leftOperand,
                      SyntaxTreeNode *rightOperand, SymbolTable &symbolTable) {
  if (leftOperand->expType != SymbolType::STRUCT) {
    printErrorMessage(13, parentExp->firstLine,
                      "accessing with non-struct variable");
    parentExp->expType = SymbolType::UNKNOWN;
  } else {
    const Symbol *symbol = leftOperand->symbol;
    const StructType *symbolData = std::get<StructType *>(symbol->symbolData);
    const std::string &varName = rightOperand->attribute_value;
    std::unordered_map<std::string, Symbol *> nameTypeMap;
    for (auto i = 0; i < symbolData->fieldName.size(); ++i) {
      nameTypeMap.insert(
              std::make_pair(symbolData->fieldName[i], symbolData->fieldType[i]));
    }
    if (nameTypeMap.find(varName) == nameTypeMap.end()) {
      printErrorMessage(14, parentExp->firstLine,
                        "no such member: " + rightOperand->attribute_value);
    } else {
      parentExp->expType = nameTypeMap[varName]->symbolType;
      parentExp->symbol = nameTypeMap[varName];
    }
  }
}

ArrayType *updateArrExpType(SyntaxTreeNode *leftExpNode, SyntaxTreeNode *rightExpNode,
                            Symbol *arraySymbol) {
  int depth = 1;
  while (rightExpNode->getChildren().size() != 1) {
    depth++;
    rightExpNode = rightExpNode->children[0];
  }

  ArrayType *data = std::get<ArrayType *>(arraySymbol->symbolData);
  int remainDepth = data->remainDepth - depth;
  while (data->remainDepth != remainDepth && remainDepth >= 0) {
    data = std::get<ArrayType *>(data->baseType->symbolData);
  }
  leftExpNode->expType = data->baseType->symbolType;

  return data;
}

bool isNumber(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

void checkIndexTypeAndRange(SyntaxTreeNode *indexExpNode, ArrayType *data, SymbolTable &symbolTable) {
  if (indexExpNode->expType != SymbolType::INT) {
    printErrorMessage(12, indexExpNode->firstLine, "indexing by non-integer");
  }else{
    std::string indexString = indexExpNode->attribute_value;
    if(isNumber(indexString)){
      int value = stoi(indexExpNode->attribute_value);
      if (value >= data->size || value < 0) {
        printErrorMessage(17, indexExpNode->firstLine, "index out of range");
      }
    }else{
      // todo: index is int type variable
    }
  }
}

void checkAndUpdateExpArray(SyntaxTreeNode *leftExpNode, SyntaxTreeNode *rightExpNode,
                            SyntaxTreeNode *indexExpNode, SymbolTable &symbolTable) {
  std::string arrayName;
  if (rightExpNode->getChildren().size() == 1) {
    arrayName = rightExpNode->children[0]->attribute_value;
  } else {
    arrayName = rightExpNode->attribute_value;
  }

  Symbol *arraySymbol = symbolTable.searchVariableSymbol(arrayName);
  if (arraySymbol != nullptr) {
    ArrayType *data = updateArrExpType(leftExpNode, rightExpNode, arraySymbol);
    checkIndexTypeAndRange(indexExpNode, data, symbolTable);
  }
  // todo: undefined variable v.s. non-array variable
  if (arraySymbol == nullptr || arraySymbol->symbolType != SymbolType::ARRAY) {
    printErrorMessage(10, rightExpNode->firstLine, "indexing on non-array variable");
  }
}
