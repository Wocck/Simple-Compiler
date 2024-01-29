#include "Parser/symbolTableManager.h"

SymbolTableManager::SymbolTableManager() {
    tables = {};
}

void SymbolTableManager::enterNewScope() {
    tables.back().emplace_back();
}

bool SymbolTableManager::leaveScope() {
    if (!tables.back().empty()) {
        tables.back().pop_back();
        return true;
    } else
        return false;
}

void SymbolTableManager::enterNewContext() {
    tables.emplace_back();
}

bool SymbolTableManager::leaveContext() {
    if(!tables.empty())
    {
        tables.pop_back();
        return true;
    }
    else
        return false;

}

std::optional<SymbolInfo> SymbolTableManager::getSymbol(const std::string &identifier, bool isFun) {
    if (isFun) {
        if (!tables.empty() && !tables.front().empty()) {
            return tables.front().front().getSymbol(identifier);
        }
        return std::nullopt;
    }

    // lokalny od najbardziej zagnieżdżonego
    if (!tables.empty() && !tables.back().empty()) {
        for (auto it = tables.back().rbegin(); it != tables.back().rend(); ++it) {
            auto symbol = it->getSymbol(identifier);
            if (symbol.has_value()) {
                return symbol;
            }
        }
    }

    // zakres globalny
    if (!tables.empty() && !tables.front().empty()) {
        return tables.front().front().getSymbol(identifier);
    }

    // nieznaleziono
    return std::nullopt;
}

bool SymbolTableManager::insertSymbol(const std::string &identifier, const SymbolInfo& symbol) {
    return tables.back().back().insert(identifier, symbol);
}

void SymbolTableManager::setValue(const std::string &identifier, const std::variant<int, float, bool, std::string> &newValue) {
    for(int i  = int(tables.back().size())-1; i>=0; i--) {
        auto val = tables.back()[i].getSymbol(identifier);
        if(val.has_value()) {
            tables.back()[i].setValue(identifier,newValue);
            return;
        }
    }
    if(tables.front().front().getSymbol(identifier).has_value())
        tables.front().front().setValue(identifier,newValue);
}

bool SymbolTableManager::isGlobal(const std::string &identifier) {
    if (tables.size() > 1)
        for (auto& table : tables.back())
            if (table.getSymbol(identifier).has_value())
                return false;

    return tables.front().front().getSymbol(identifier).has_value();
}

bool SymbolTableManager::checkIfExists(const std::string &identifier) {
    if (!tables.empty()) {
        for (auto it = tables.rbegin(); it != tables.rend(); ++it) {
            for (auto& table : *it) {
                if (table.getSymbol(identifier).has_value()) {
                    return true; // Znaleziono symbol w lokalnym zakresie.
                }
            }
        }
    }

    if (!tables.empty() && !tables.front().empty()) {
        return tables.front().front().getSymbol(identifier).has_value();
    }

    return false;
}

