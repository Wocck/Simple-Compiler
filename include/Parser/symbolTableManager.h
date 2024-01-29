#ifndef TKOM_PROJEKT_SYMBOLTABLEMANAGER_H
#define TKOM_PROJEKT_SYMBOLTABLEMANAGER_H

#include "symbolTable.h"

class SymbolTableManager {
private:
    std::vector<std::vector<SymbolTable>> tables;

public:
    SymbolTableManager();

    void enterNewScope();
    bool leaveScope();
    void enterNewContext();
    bool leaveContext();
    std::optional<SymbolInfo> getSymbol(const std::string&, bool);
    bool insertSymbol(const std::string&, const SymbolInfo&);
    void setValue(const std::string& ID, const std::variant<int, float, bool, std::string>& newValue);
    bool isGlobal(const std::string&);
    bool checkIfExists(const std::string&);
};

#endif //TKOM_PROJEKT_SYMBOLTABLEMANAGER_H
