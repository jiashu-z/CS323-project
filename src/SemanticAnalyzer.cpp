//
// Created by maozunyao on 2021/10/24.
//
#include "SemanticAnalyzer.h"
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
  Symbol *symbol = symbolTable.searchVariableSymbol(idNode->attribute_value);
  if (symbol == nullptr) {
    std::string errorMessage = "undefined variable: " + idNode->attribute_value;
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
          StructType *data = new StructType;
          symbol = new Symbol(symbolName, symbolType, data);
          symbolTable.insertVariableSymbol(symbolName, symbol);
          break;
        }
        default: { break; }
      }
    } else {
      std::string errorMessage = "redefine variable: " +
                                 dec->children[0]->children[0]->attribute_value;
      printErrorMessage(3, dec->getChildren()[0]->children[0]->firstLine,
                        errorMessage);
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
  SymbolType expType = specifier->expType;
  SyntaxTreeNode *id = paraDec->children[1]->children[0];
  std::string varName = id->attribute_value;

  switch (expType) {
    case SymbolType::INT: {
      IntType *intType = new IntType;
      Symbol *symbol = new Symbol(varName, expType, intType);
      functionType->argsType.push_back(symbol);
      bool rt = symbolTable.insertVariableSymbol(varName, symbol);
      if (!rt) {
        std::string errorMessage = "redefine variable: " + id->attribute_value;
        printErrorMessage(3, id->firstLine, errorMessage);
      } else {
        if (customDebug) {
          std::cout << varName << " is inserted" << std::endl;
        }
      }
      break;
    }
    case SymbolType::FLOAT: {
      FloatType *floatType = new FloatType;
      Symbol *symbol = new Symbol(varName, expType, floatType);
      functionType->argsType.push_back(symbol);
      bool rt = symbolTable.insertVariableSymbol(varName, symbol);
      if (!rt) {
        std::string errorMessage = "redefine variable: " + id->attribute_value;
        printErrorMessage(3, id->firstLine, errorMessage);
      }
      break;
    }
    case SymbolType::CHAR: {
      CharType *charType = new CharType;
      Symbol *symbol = new Symbol(varName, expType, charType);
      functionType->argsType.push_back(symbol);
      bool rt = symbolTable.insertVariableSymbol(varName, symbol);
      if (!rt) {
        std::string errorMessage = "redefine variable: " + id->attribute_value;
        printErrorMessage(3, id->firstLine, errorMessage);
      }
      break;
    }
    default: {
      std::cout << "array and struct type are not supported yet > <"
                << std::endl;
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
    exit(0);
  }
}

void insertStructDefinitionSymbol(SyntaxTreeNode *structSpecifierNode, SymbolTable &symbolTable) {
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

      while (decListNode->children.size() != 1) {
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
            // TODO
            break;
          }
          case SymbolType::STRUCT: {
            std::string &typeName = specifierNode->children[0]->children[1]->attribute_value;
            if (structDefinitionTable.find(typeName) != structDefinitionTable.end()) {
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
        decListNode = decListNode->children[2];
      }
      SymbolType &expType = specifierNode->expType;
      
      defListNode = defListNode->children[1];
    }
    Symbol *symbol = new Symbol(name, symbolType, data);
    structDefinitionTable.insert(std::make_pair(typeName, symbol));
  } else {
    printErrorMessage(15, structSpecifierNode->firstLine, "todo");
  }
}

void insertStructSymbol(SyntaxTreeNode *defNode, SymbolTable &symbolTable) {
  
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
  for (auto &i : node->children) {
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