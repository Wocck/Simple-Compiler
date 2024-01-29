#include "symbolTable.h"

SymbolInfo::SymbolInfo(const std::string &identifier, const std::variant<IdType, std::string, std::shared_ptr<StructInfo>>& type,
                       bool isFunction, bool isMutable, bool isSimpleType,
                       const std::optional<std::variant<std::variant<int, float, bool, std::string>, Nodes::FunctionDeclaration *>>& value) {
    this->identifier = identifier;
    this->isMutable = isMutable;
    this->isFunction = isFunction;
    this->isSimpleType = isSimpleType;
    this->type = type;
    if (isFunction)
        this->funcPointer = std::get<Nodes::FunctionDeclaration *>(value.value());
    else if (value.has_value())
        this->value = std::get<std::variant<int, float, bool, std::string>>(value.value());
}

std::string SymbolInfo::getIdentifier() const {
    return identifier;
}

std::variant<IdType, std::string> SymbolInfo::getType() const {
    if (isSimpleType)
        return std::get<IdType>(type);
    else
        return std::get<std::string>(type);
}

bool SymbolInfo::isFun() const {
    return isFunction;
}

bool SymbolInfo::isMut() const {
    return isMutable;
}

bool SymbolInfo::isSimple() const {
    return isSimpleType;
}

std::string SymbolInfo::getTypeAsString() const {
    if (isSimpleType)
        return Nodes::idTypesToStr[std::get<IdType>(type)];

    return std::get<std::string>(type);
}

bool SymbolInfo::isStruct() const {
    return std::holds_alternative<std::shared_ptr<StructInfo>>(type);
}

std::shared_ptr<StructInfo> SymbolInfo::getStructInfo() const {
    if (isStruct()) {
        return std::get<std::shared_ptr<StructInfo>>(type);
    }
    return nullptr;
}

std::optional<std::variant<int, float, bool, std::string>> SymbolInfo::getValue() const {
    return value;
}

SymbolTable::SymbolTable() {
    table = {};
}

bool SymbolTable::insert(const std::string &identifier, const SymbolInfo& symbol) {
    if (table.find(identifier) != table.end())
        return false;
    table.insert(std::make_pair(identifier, symbol));
    return true;
}

std::optional<SymbolInfo> SymbolTable::getSymbol(const std::string &identifier) {
    auto found = table.find(identifier);
    if (found == table.end())
        return std::nullopt;
    return found->second;
}

bool SymbolTable::setValue(const std::string &identifier, std::variant<int, float, bool, std::string> newValue) {
    auto symbol = table.find(identifier);
    if (symbol == table.end())
        return false;

    symbol->second.setValue(newValue);
    return true;
}
