//
// Created by maozunyao on 2021/10/24.
//

#include "SymbolTable.h"


bool SymbolTable::insertSymbol(std::string name, Symbol *symbol) {
     if(this->currentTable.find(name)==this->currentTable.end()) {
        this->currentTable.emplace(name, symbol);
        return true;
    }
    else{
        return false;
    }
}

Symbol* SymbolTable::searchSymbol(std::string name) {
    auto it=this->currentTable.find(name);
    if(it!=currentTable.end()){
        return it->second;
    } else{
        return nullptr;
    }
}

Symbol::Symbol(const std::string &name, SymbolType symbolType, const Symbol::DATA &data) : name(name),
                                                                                           symbolType(symbolType),
                                                                                           data(data) {}
