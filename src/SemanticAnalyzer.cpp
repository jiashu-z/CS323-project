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
void insertVarListToFunctionType(FunctionType * functionType,SyntaxTreeNode * varList,SymbolTable&symbolTable){
    SyntaxTreeNode * paraDec=varList->children[0];
    SyntaxTreeNode * specifier=paraDec->children[0];
    SymbolType expType=specifier->expType;
    SyntaxTreeNode * id=paraDec->children[1]->children[0];
    std::string varName=id->attribute_value;

        switch (expType) {
            case SymbolType::INT: {
                IntType *intType = new IntType;
                Symbol *symbol = new Symbol(varName, expType, intType);
                functionType->argsType.push_back(symbol);
                bool rt=symbolTable.insertVariableSymbol(varName,symbol);
                if(!rt){
                    std::cout << "Error Type " << 3 << " at Line "
                              << id->firstLine << ": "
                              << "Redefined variable \""
                              << id->attribute_value << "\"."
                              << std::endl;
                }
                else{
                    if(customDebug){
                        std::cout<<varName<<" is inserted"<<std::endl;
                    }
                }
                break;
            }
            case SymbolType::FLOAT: {
                FloatType *floatType = new FloatType;
                Symbol *symbol = new Symbol(varName, expType, floatType);
                functionType->argsType.push_back(symbol);
                bool rt=symbolTable.insertVariableSymbol(varName,symbol);
                if(!rt){
                    std::cout << "Error Type " << 3 << " at Line "
                              << id->firstLine << ": "
                              << "Redefined variable \""
                              << id->attribute_value << "\"."
                              << std::endl;
                }
                break;
            }
            case SymbolType::CHAR: {
                CharType *charType = new CharType;
                Symbol *symbol = new Symbol(varName, expType, charType);
                functionType->argsType.push_back(symbol);
                bool rt=symbolTable.insertVariableSymbol(varName,symbol);
                if(!rt){
                    std::cout << "Error Type " << 3 << " at Line "
                              << id->firstLine << ": "
                              << "Redefined variable \""
                              << id->attribute_value << "\"."
                              << std::endl;
                }
                break;
            }
            default: {
                std::cout << "array and struct type are not supported yet > <" << std::endl;
            }
        }

    if(varList->children.size()==3){
        SyntaxTreeNode * childVarList=varList->getChildren()[2];
        insertVarListToFunctionType(functionType,childVarList,symbolTable);
    }
}
void insertFunctionSymbolWithArgs(SyntaxTreeNode* funDec,SymbolTable &symbolTable){
    std::string functionName=funDec->children[0]->attribute_value;
    FunctionType *functionType=new FunctionType;
    Symbol *rt=symbolTable.searchFunctionSymbol(functionName);
    SyntaxTreeNode * varList=funDec->children[2];
    if(rt== nullptr) {
        functionType->functionName = functionName;
        functionType->returnType = funDec->expType;
        insertVarListToFunctionType(functionType,varList,symbolTable);
        Symbol *symbol = new Symbol(functionName, funDec->expType, functionType);
        symbolTable.insertFunctionSymbol(functionName,symbol);
    }
    else{
        std::cout<<"Error type 4 at Line "<<funDec->firstLine<<": multi definition of function "<<functionName<<std::endl;
        exit(2);
    }
}

void insertFunctionSymbol(SyntaxTreeNode *funDec,
                          SymbolTable &symbolTable) {

//    if(symbolTable.searchFunctionSymbol(funDec->getChildren()[0]->attribute_value)!= nullptr){
//
//    }
  //  else {
        if (funDec->children.size() == 3) {
            insertFunctionSymbolWithoutArgs(funDec, symbolTable);
        } else if (funDec->children.size() == 4) {
            insertFunctionSymbolWithArgs(funDec, symbolTable);
        }
 //   }

}

