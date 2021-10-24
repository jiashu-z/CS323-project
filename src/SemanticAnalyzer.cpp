//
// Created by maozunyao on 2021/10/24.
//
#include "SemanticAnalyzer.h"
extern int customDebug;
void usePrimarySymbol(SyntaxTreeNode * idNode, SymbolTable &symbolTable){
Symbol * symbol = symbolTable.searchSymbol(idNode->attribute_value);
if(symbol==nullptr){
std::cout << "Error Type " << 1 << " at Line " << idNode->firstLine << ": " << "Undefined variable \"" << idNode->attribute_value << "\"." << std::endl;
}
}
void insertPrimarySymbol(SyntaxTreeNode* defNode,SymbolTable &symbolTable){
    SyntaxTreeNode *specifier=defNode->children[0];
    SyntaxTreeNode *decList=defNode->children[1];
    // multiDec not handled
    SyntaxTreeNode *dec=decList->children[0];
    std::string symbolName=dec->children[0]->children[0]->attribute_value;

    Symbol * returnPtr=symbolTable.searchSymbol(symbolName);
    if(returnPtr== nullptr) {

        SymbolType symbolType = dec->children[2]->expType;
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
        if(symbol== nullptr){
            std::cout<<"fatal unhandled error!"<<std::endl;
        }else{
            if(customDebug){
                std::cout<<"Symbol "<<symbolName<<" inserted"<<std::endl;
            }
            symbolTable.insertSymbol(symbolName,symbol);
        }
    }
    else{
        std::cout << "Error Type " << 3 << " at Line " << dec->children[0]->children[0]->firstLine << ": " << "Redefined variable \"" << dec->children[0]->children[0]->attribute_value << "\"." << std::endl;
    }
}