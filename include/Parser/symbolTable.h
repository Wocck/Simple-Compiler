#ifndef TKOM_PROJEKT_SYMBOLTABLE_H
#define TKOM_PROJEKT_SYMBOLTABLE_H

#include <optional>
#include "syntaxTree.h"

class StructInfo;
class StructFieldInfo;

class StructInfo
{
public:
    std::map<std::string, StructFieldInfo> fields;
    explicit StructInfo(const std::map<std::string, StructFieldInfo>& fields) : fields(fields) {}
};

class StructFieldInfo
{
private:
    IdType fieldType;
    std::optional<std::variant<int, float, bool, std::string>> fieldValue;

public:
    StructFieldInfo(const IdType fieldType,
                    const std::optional<std::variant<int, float, bool, std::string>> fieldValue)
            : fieldType(fieldType), fieldValue(fieldValue) {}
    StructFieldInfo() : fieldType(IdType::STRUCT) {};
    [[nodiscard]] IdType getFieldType() const { return fieldType; }
    [[nodiscard]] std::optional<std::variant<int, float, bool, std::string>> getFieldValue() const { return fieldValue; }
};

class SymbolInfo
{
private:
    std::string identifier;
    std::variant<IdType, std::string, std::shared_ptr<StructInfo>> type;
    bool isFunction;
    bool isMutable;
    bool isSimpleType;
    std::optional<std::variant<int,float,bool,std::string>> value;
    std::optional<Nodes::FunctionDeclaration*> funcPointer;
public:
    SymbolInfo(const std::string& identifier, const std::variant<IdType,
               std::string, std::shared_ptr<StructInfo>>& type, bool isFunction, bool isMutable, bool isSimpleType,
               const std::optional<std::variant<std::variant<int,float,bool,std::string>, Nodes::FunctionDeclaration*>>& value);

    std::optional<Nodes::FunctionDeclaration*> getFuncPointer() {return funcPointer.value();}
    [[nodiscard]] std::string getIdentifier() const;
    [[nodiscard]] std::variant<IdType, std::string> getType() const;
    [[nodiscard]] std::string getTypeAsString() const;
    [[nodiscard]] bool isFun() const;
    [[nodiscard]] bool isMut() const;
    [[nodiscard]] bool isSimple() const;
    [[nodiscard]] bool isStruct() const;
    [[nodiscard]] std::shared_ptr<StructInfo> getStructInfo() const;
    void setValue(std::variant<int,float,bool,std::string> &val) { this->value=val;}
    [[nodiscard]] std::optional<std::variant<int,float,bool,std::string>> getValue() const;
};

class SymbolTable
{
private:
    std::map<std::string, SymbolInfo> table;
public:
    SymbolTable();
    bool insert(const std::string& identifier, const SymbolInfo& symbol);
    std::optional<SymbolInfo> getSymbol(const std::string& identifier);
    bool setValue(const std::string& identifier, std::variant<int,float,bool,std::string> newValue);
};

#endif //TKOM_PROJEKT_SYMBOLTABLE_H
