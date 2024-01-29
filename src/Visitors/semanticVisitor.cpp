#include <set>
#include "semanticVisitor.h"

void SemanticVisitor::visitBoolLiteral(Nodes::BooleanLiteral *literal) {
    lastEvaluatedType = IdType::BOOLEAN;
    if (expectedType == std::nullopt)
        return;
    if (this->getExpectedTypeAsString() != Nodes::idTypesToStr[IdType::BOOLEAN]){
        std::string expectedTypeStr = this->getExpectedTypeAsString();
        throw MyException("Expected type: " + expectedTypeStr + ", got: BOOLEAN", literal->getPos());
    }
}

void SemanticVisitor::visitIntLiteral(Nodes::IntLiteral *literal) {
    lastEvaluatedType = IdType::INT;
    if (expectedType == std::nullopt)
        return;
    if (this->getExpectedTypeAsString() != Nodes::idTypesToStr[IdType::INT]){
        std::string expectedTypeStr = this->getExpectedTypeAsString();
        throw MyException("Expected type: " + expectedTypeStr + ", got: INT", literal->getPos());
    }
}

void SemanticVisitor::visitFloatLiteral(Nodes::FloatLiteral *literal) {
    lastEvaluatedType = IdType::FLOAT;
    if (expectedType == std::nullopt)
        return;
    if (this->getExpectedTypeAsString() != Nodes::idTypesToStr[IdType::FLOAT]){
        std::string expectedTypeStr = this->getExpectedTypeAsString();
        throw MyException("Expected type: " + expectedTypeStr + ", got: FLOAT", literal->getPos());
    }
}

void SemanticVisitor::visitStringLiteral(Nodes::StringLiteral *literal) {
    lastEvaluatedType = IdType::STR;
    if (expectedType == std::nullopt)
        return;
    if (this->getExpectedTypeAsString() != Nodes::idTypesToStr[IdType::STR]){
        std::string expectedTypeStr = this->getExpectedTypeAsString();
        throw MyException("Expected type: " + expectedTypeStr + ", got: STRING", literal->getPos());
    }

}

void SemanticVisitor::visitIdentifier(Nodes::Identifier *identifier) {
    // zmienna
    auto symbol = symbolManager.getSymbol(identifier->getName(), false);
    if (symbol.has_value())
    {
        lastEvaluatedType = std::get<IdType>(symbol.value().getType());
        return;
    }
    // funkcja
    symbol = symbolManager.getSymbol(identifier->getName(), true);
    if (symbol.has_value())
    {
        lastEvaluatedType = std::get<IdType>(symbol.value().getType());
        return;
    }
    throw MyException("Unknown variable (cannot use struct/variant)" + identifier->getName(), identifier->getPos());
}

void SemanticVisitor::visitRelOp(Nodes::RelOp *) {}
void SemanticVisitor::visitArtmOp(Nodes::ArtmOp *) {}
void SemanticVisitor::visitFactorOp(Nodes::FactorOp *) {}
void SemanticVisitor::visitUnaryOp(Nodes::UnaryOp *) {}
void SemanticVisitor::visitCastOp(Nodes::CastOp *) {}

void SemanticVisitor::visitCastingExpr(Nodes::CastingExpr *castingExpr) {
    castingExpr->acceptCastOperator(*this);
    auto prevExpectedType = expectedType;
    expectedType = std::nullopt;
    castingExpr->acceptExpr(*this);
    if (castingExpr->getCastOp())
    {
        auto op = castingExpr->getCastOp()->getType();
        if (op == IdType::BOOLEAN && lastEvaluatedType == IdType::STR)
            throw MyException("Cannot cast STR to BOOLEAN", castingExpr->getPos());
        lastEvaluatedType = op;
    }
    expectedType = prevExpectedType;
}

void SemanticVisitor::visitUnaryExpr(Nodes::UnaryExpr *unaryExpr) {
    unaryExpr->acceptUnaryOperator(*this);
    unaryExpr->acceptExpr(*this);
    if (unaryExpr->getUnaryOp())
    {
        auto op = unaryExpr->getUnaryOp()->getType();
        if (op == UnaryOperator::NEGATE) {
            if (lastEvaluatedType != IdType::BOOLEAN)
                throw MyException("Cannot use NOT operator on non-boolean type", unaryExpr->getPos());
        } else if (op == UnaryOperator::NEGATIVE) {
            if (lastEvaluatedType != IdType::INT && lastEvaluatedType != IdType::FLOAT)
                throw MyException("Cannot use MINUS operator on non-numeric type", unaryExpr->getPos());
        }
    }
}

void SemanticVisitor::visitMulExpr(Nodes::MulExpr *mulExpr) {
    mulExpr->acceptLeft(*this);
    auto leftType = lastEvaluatedType;
    mulExpr->acceptFactorOperator(*this);
    mulExpr->acceptRight(*this);
    auto rightType = lastEvaluatedType;

    if (mulExpr->getLeftOperand() && mulExpr->getRightOperand())
    {
        if (leftType.value() != rightType.value())
            throw MyException("Cannot perform arithmetic operation on different types", mulExpr->getPos());
        if (leftType.value() == IdType::STR || rightType.value() == IdType::STR)
            throw MyException("Cannot perform arithmetic operation on strings", mulExpr->getPos());
        if (leftType.value() == IdType::BOOLEAN || rightType.value() == IdType::BOOLEAN )
            throw MyException("Cannot perform arithmetic operation on booleans", mulExpr->getPos());
    }
}

void SemanticVisitor::visitArtmExpr(Nodes::ArtmExpr *artmExpr) {
    artmExpr->acceptLeft(*this);
    auto leftType = lastEvaluatedType;
    artmExpr->acceptArtmOperator(*this);
    artmExpr->acceptRight(*this);
    auto rightType = lastEvaluatedType;
    if (artmExpr->getLeftOperand() && artmExpr->getRightOperand())
    {
        auto op = artmExpr->getArtmOp()->getType();
        if (leftType.value() != rightType.value())
            throw MyException("Cannot perform arithmetic operation on different types", artmExpr->getPos());
        if (leftType.value() == IdType::STR && rightType.value() == IdType::STR && op != ArtmOperator::PLUS)
            throw MyException("Cannot perform arithmetic operation on strings", artmExpr->getPos());
        if (leftType.value() == IdType::BOOLEAN || rightType.value() == IdType::BOOLEAN )
            throw MyException("Cannot perform arithmetic operation on booleans", artmExpr->getPos());
        if (leftType.value() == rightType.value())
            lastEvaluatedType = leftType;
    }

}

void SemanticVisitor::visitRelExpr(Nodes::RelExpr *relExpr) {
    relExpr->acceptLeft(*this);
    auto leftType = lastEvaluatedType;
    relExpr->acceptRelOperator(*this);
    relExpr->acceptRight(*this);
    auto rightType = lastEvaluatedType;
    if (relExpr->getLeftOperand() && relExpr->getRightOperand())
    {
        auto op = relExpr->getRelOp()->getType();
        if (leftType.value() != rightType.value())
            throw MyException("Cannot compare different types", relExpr->getPos());
        if (leftType.value() == IdType::STR && rightType.value() == IdType::STR )
            if (op != RelationalOperator::EQUAL && op != RelationalOperator::NOT_EQUAL)
                throw MyException("Cannot use relational operator on strings", relExpr->getPos());
    }
}

void SemanticVisitor::visitAndExpr(Nodes::AndExpr *andExpr) {
    andExpr->acceptLeft(*this);
    auto leftType = lastEvaluatedType;
    andExpr->acceptRight(*this);
    auto rightType = lastEvaluatedType;
    if (andExpr->getLeftOperand() && andExpr->getRightOperand())
    {
        if (leftType.value() == IdType::STR || rightType.value() == IdType::STR)
            throw MyException("Cannot use AND operator on strings", andExpr->getPos());
    }
}

void SemanticVisitor::visitOrExpr(Nodes::OrExpr *orExpr) {
    orExpr->acceptLeft(*this);
    auto leftType = lastEvaluatedType;
    orExpr->acceptRight(*this);
    auto rightType = lastEvaluatedType;
    if (orExpr->getLeftOperand() && orExpr->getRightOperand())
    {
        if (leftType.value() == IdType::STR || rightType.value() == IdType::STR)
            throw MyException("Cannot use OR operator on strings", orExpr->getPos());
    }
}

void SemanticVisitor::visitExpr(Nodes::Expression *expr) {
    auto prevType = expectedType;
    expectedType=std::nullopt;
    expr->acceptExpr(*this);
    expectedType = prevType;
}

void SemanticVisitor::visitFuncCall(Nodes::FunCall *funCall) {
    if (funCall->getIdentifier() == "print") {
        throw MyException("Cannot call print function as value", funCall->getPos());
    }

    auto symbol = symbolManager.getSymbol(funCall->getIdentifier(), true);

    if (!symbol.has_value()) {
        throw MyException("Function " + funCall->getIdentifier() + " not declared", funCall->getPos());
    }
    if (!symbol.value().isFun()) {
        throw MyException("Cannot call non-function " + funCall->getIdentifier(), funCall->getPos());
    }

    if (expectedType != std::nullopt && symbol.value().getTypeAsString() != this->getExpectedTypeAsString()) {
        throw MyException(
                "Expected type: " + this->getExpectedTypeAsString() +
                ", got: " + symbol.value().getTypeAsString(),
                funCall->getPos());
    }

    auto funArgs = symbol.value().getFuncPointer().value()->getParameters();
    auto calledArgs = funCall->getArguments();
    if (!funArgs.has_value() || !calledArgs.has_value() || funArgs.value().size() != calledArgs.value().size()) {
        throw MyException("Function " + funCall->getIdentifier() + " called with wrong number of arguments", funCall->getPos());
    }

    for (int i = 0; i < funArgs.value().size(); i++) {
        auto prevExpectedType = expectedType;
        expectedType = funArgs.value()[i]->getType()->getIdType();
        calledArgs.value()[i]->accept(*this);
        expectedType = prevExpectedType;
    }
}

void SemanticVisitor::visitVariableRef(Nodes::VarReference *varReference) {
    auto symbol = symbolManager.getSymbol(varReference->getIdentifier(), false);
    if (!symbol.has_value()) {
        throw MyException("Variable " + varReference->getIdentifier() + " not declared", varReference->getPos());
    }
    if (symbol.value().isFun()) {
        throw MyException("Cannot use function " + varReference->getIdentifier() + " as value", varReference->getPos());
    }
    if (expectedType.has_value() && symbol.value().getTypeAsString() != this->getExpectedTypeAsString()) {
        throw MyException(
                "Expected type: " + this->getExpectedTypeAsString() +
                ", got: " + symbol.value().getTypeAsString(),
                varReference->getPos());
    }
    lastEvaluatedType = std::get<IdType>(symbol.value().getType());
}

void SemanticVisitor::visitDeclaration(Nodes::Declaration *declaration) {}
void SemanticVisitor::visitType(Nodes::Type *) {}

void SemanticVisitor::visitTypeDecl(Nodes::TypeDecl *typeDecl) {
    std::variant<int, float, bool, std::string, std::shared_ptr<StructInfo>> value;
    auto type = std::get<IdType>(typeDecl->getType()->getIdType());
    switch (type) {
        case IdType::INT:
            value.emplace<int>(0);
            break;
        case IdType::FLOAT:
            value.emplace<float>(0.0);
            break;
        case IdType::BOOLEAN:
            value.emplace<bool>(false);
            break;
        case IdType::STR:
            value.emplace<std::string>("");
            break;
    }

    if(!symbolManager.insertSymbol(typeDecl->getIdentifier(), SymbolInfo(typeDecl->getIdentifier(), type, false, true, true, std::nullopt)))
    {
        throw MyException("Variable redefinition: " + typeDecl->getIdentifier(), typeDecl->getPos());
    }

}

void SemanticVisitor::visitVariableDeclaration(Nodes::VariableDeclaration *variableDeclaration) {
    std::string identifier = variableDeclaration->getIdentifier();
    if (structTypes.find(identifier) != structTypes.end()) {
        throw MyException("Variable name'" + identifier + "' is already declared as struct type", variableDeclaration->getPos());
    }
    if (variantTypes.find(identifier) != variantTypes.end()) {
        throw MyException("Variable name'" + identifier + "' is already declared as variant type", variableDeclaration->getPos());
    }

    try {
        std::get<IdType>(variableDeclaration->getTypeDecl()->getType()->getIdType());
    } catch (std::bad_variant_access&) {
        throw MyException("Semantic error: Type '" + identifier + "' is not a simple type", variableDeclaration->getPos());
    }
    auto type = std::get<IdType>(variableDeclaration->getTypeDecl()->getType()->getIdType());
    std::variant<int, float, bool, std::string> value;
    switch (type) {
        case IdType::INT:
            value.emplace<int>(0);
            break;
        case IdType::FLOAT:
            value.emplace<float>(0.0);
            break;
        case IdType::BOOLEAN:
            value.emplace<bool>(false);
            break;
        case IdType::STR:
            value.emplace<std::string>("");
            break;
    }
    if (!symbolManager.insertSymbol(identifier, SymbolInfo(identifier, type, false, variableDeclaration->isMutable(), true, value))) {
        throw MyException("Semantic error: Variable '" + identifier + "' is already declared in this scope", variableDeclaration->getPos());
    }

    if (variableDeclaration->getInitExpr()) {
        std::optional<std::variant<IdType, std::string>> previousType = std::nullopt;
        if (expectedType.has_value())
            previousType = expectedType;
        expectedType = type;
        variableDeclaration->getInitExpr()->acceptExpr(*this);
        expectedType = previousType;
    }
}

void SemanticVisitor::visitStructTypeDefinition(Nodes::StructTypeDefinition *structTypeDefinition) {
    std::string structIdentifier = structTypeDefinition->getStructName();
    if (symbolManager.checkIfExists(structIdentifier)) {
        throw MyException("Variable '" + structIdentifier + "' is already declared", structTypeDefinition->getPos());
    }
    if (structTypeDefinition->getFields().empty())
        throw MyException("Struct Type '" + structIdentifier + "' cannot be empty", structTypeDefinition->getPos());
    std::set<std::string> fieldNames;
    for (const auto& field : structTypeDefinition->getFields()) {
        auto fieldTypeName = field->getType()->getTypeAsString();
        auto fieldName = field->getIdentifier();
        if (!fieldNames.insert(fieldName).second) {
            throw MyException("Duplicate field name '" + fieldName + "' in struct '" + structIdentifier + "'", field->getPos());
        }
        if (fieldName == structIdentifier) {
            throw MyException("Struct '" + structIdentifier + "' fields cannot be named as struct itself", field->getPos());
        }
        if (fieldTypeName == structIdentifier) {
            throw MyException("Struct '" + structIdentifier + "' cannot contain itself", field->getPos());
        }
        if (!doesTypeExist(fieldTypeName)) {
            throw MyException("Struct '" + structIdentifier + "' contains unknown type '" + fieldTypeName + "' (maybe struct or variant)", field->getPos());
        }
    }
}

void SemanticVisitor::visitStructVarDeclaration(Nodes::StructVarDeclaration *structVarDeclaration) {
    std::string varName = structVarDeclaration->getIdentifier();
    std::string structTypeName = structVarDeclaration->getTypeName();
    bool isMutable = structVarDeclaration->isMutable();
    if (structTypes.find(structTypeName) == structTypes.end())
        throw MyException("Struct type '" + structTypeName + "' not declared", structVarDeclaration->getPos());
    if (symbolManager.checkIfExists(varName)) {
        throw MyException("Variable '" + varName + "' is already declared", structVarDeclaration->getPos());
    }
    auto structValues = structVarDeclaration->getArgs();
    auto &structAgrTypes = structTypes.at(structTypeName)->getFields();
    if (structAgrTypes.size() != structValues.size()) {
        throw MyException("Struct '" + structTypeName + "' called with wrong number of arguments", structVarDeclaration->getPos());
    }
    for (int i = 0; i < structValues.size(); i++) {
        auto prevExpectedType = expectedType;
        expectedType = structAgrTypes[i]->getType()->getIdType();
        structValues[i]->accept(*this);
        expectedType = prevExpectedType;
    }

    std::map<std::string, StructFieldInfo> structInstanceFields;
    for (const auto & structAgrType : structAgrTypes) {
        auto fieldType = std::get<IdType>(structAgrType->getType()->getIdType());
        std::optional<std::variant<int, float, bool, std::string>> fieldValue;

        switch (fieldType) {
            case IdType::INT:
                fieldValue.emplace(0);
                break;
            case IdType::FLOAT:
                fieldValue.emplace(0.0f);
                break;
            case IdType::BOOLEAN:
                fieldValue.emplace(false);
                break;
            case IdType::STR:
                fieldValue.emplace(std::string(""));
                break;
        }
        structInstanceFields[structAgrType->getIdentifier()] = StructFieldInfo(fieldType, fieldValue);
    }

    auto structInfo = std::make_shared<StructInfo>(structInstanceFields);

    SymbolInfo structVarSymbol(varName, structInfo, false, isMutable, false, std::nullopt);

    if (!symbolManager.insertSymbol(varName, structVarSymbol)) {
        throw MyException("Error inserting struct variable '" + varName + "' into symbol table", structVarDeclaration->getPos());
    }
}

void SemanticVisitor::visitVariantTypeDefinition(Nodes::VariantTypeDefinition *variantTypeDefinition) {
    std::string variantIdentifier = variantTypeDefinition->getVariantName();
    if (symbolManager.checkIfExists(variantIdentifier)) {
        throw MyException("Variable '" + variantIdentifier + "' is already declared", variantTypeDefinition->getPos());
    }
    if (variantTypeDefinition->getFields().empty())
        throw MyException("Variant Type '" + variantIdentifier + "' cannot be empty", variantTypeDefinition->getPos());
    for (const auto& field : variantTypeDefinition->getFields()) {
        auto fieldTypeName = field->getTypeAsString();
        if (fieldTypeName == variantIdentifier) {
            throw MyException("Variant '" + variantIdentifier + "' cannot contain itself", field->getPos());
        }
        if (!doesTypeExist(fieldTypeName)) {
            throw MyException("Variant '" + variantIdentifier + "' contains unknown type '" + fieldTypeName + "' (maybe struct or variant)", field->getPos());
        }
    }

}

void SemanticVisitor::visitVariantVarDeclaration(Nodes::VariantVarDeclaration *variantVarDeclaration) {
    std::string varName = variantVarDeclaration->getIdentifier();
    std::string variantTypeName = variantVarDeclaration->getTypeName();
    if (variantTypes.find(variantTypeName) == variantTypes.end())
        throw MyException("Variant type '" + variantTypeName + "' not declared", variantVarDeclaration->getPos());
    if (symbolManager.checkIfExists(varName))
        throw MyException("Variable '" + varName + "' is already declared", variantVarDeclaration->getPos());
    auto &variantArgs = variantTypes.at(variantTypeName)->getFields();
    auto value = variantVarDeclaration->getValue();
    if (value) {
        value->accept(*this);
        bool typeMatchFound = false;
        for (const auto &argType: variantTypes.at(variantTypeName)->getFields()) {
            if (std::get<IdType>(argType->getIdType()) == lastEvaluatedType) {
                typeMatchFound = true;
                break;
            }
        }
        if (!typeMatchFound) {
            throw MyException("Type of expression does not match any of the allowed variant types",
                              variantVarDeclaration->getPos());
        }
    }
    // value set in interpreter
    SymbolInfo variantVarSymbol(varName, variantTypeName, false, true, false, std::nullopt);
    symbolManager.insertSymbol(varName, variantVarSymbol);
}

void SemanticVisitor::visitAssignment(Nodes::Assignment *assignment) {
    auto symbol = symbolManager.getSymbol(assignment->getIdentifier(), false);
    if (!symbol.has_value()) {
        throw MyException("Undefined Variable:" + assignment->getIdentifier(), assignment->getPos());
    }
    if (symbol.value().isFun()) {
        throw MyException("Cannot assign value to function " + assignment->getIdentifier(), assignment->getPos());
    }
    if (!symbol.value().isMut()) {
        throw MyException("Cannot assign value to immutable variable " + assignment->getIdentifier(), assignment->getPos());
    }
    auto prevExpectedType = expectedType;
    expectedType = symbol.value().getType();
    assignment->acceptExpr(*this);
    expectedType = prevExpectedType;

}

void SemanticVisitor::visitStructFieldAssignment(Nodes::StructFieldAssignment *structFieldAssignment) {
    auto structSymbol = symbolManager.getSymbol(structFieldAssignment->getIdentifier(), false);
    if (!structSymbol.has_value()) {
        throw MyException("Struct not found: " + structFieldAssignment->getIdentifier(), structFieldAssignment->getPos());
    }
    if (!structSymbol.value().isStruct()) {
        throw MyException("Identifier is not a struct: " + structFieldAssignment->getIdentifier(), structFieldAssignment->getPos());
    }

    auto structInfo = structSymbol.value().getStructInfo();
    auto fieldName = structFieldAssignment->getFieldName();
    if (structInfo->fields.find(fieldName) == structInfo->fields.end()) {
        throw MyException("Field not found in struct: " + fieldName, structFieldAssignment->getPos());
    }

    auto fieldInfo = structInfo->fields[fieldName];
    auto fieldType = fieldInfo.getFieldType();
    auto prevExpectedType = expectedType;
    expectedType = fieldType;
    structFieldAssignment->getExpression()->accept(*this);
    expectedType = prevExpectedType;

    // The actual assignment of the value will be done in the interpreter
}

void SemanticVisitor::visitReturnStatement(Nodes::ReturnStatement *returnStatement) {
    returnStatement->acceptReturnExpr(*this);
}

void SemanticVisitor::visitBlock(Nodes::Block *block) {
    symbolManager.enterNewScope();
    block->acceptStatements(*this);
    symbolManager.leaveScope();
}

void SemanticVisitor::visitIfStatement(Nodes::IfStatement *ifStatement) {
    ifStatement->acceptCondition(*this);
    if (lastEvaluatedType == IdType::STR)
        throw MyException("Cannot use string as condition", ifStatement->getPos());
    ifStatement->acceptIfBlock(*this);
    ifStatement->acceptElseBlock(*this);
}

void SemanticVisitor::visitWhileStatement(Nodes::WhileStatement *whileStatement) {
    whileStatement->acceptCondition(*this);
    if (lastEvaluatedType == IdType::STR)
        throw MyException("Cannot use string as condition", whileStatement->getPos());
    whileStatement->acceptWhileBlock(*this);
}

void SemanticVisitor::visitFunctionCallStatement(Nodes::FunctionCallStatement *functionCallStatement) {
    auto symbol = symbolManager.getSymbol(functionCallStatement->getFunctionName(), true);
    if (functionCallStatement->getFunctionName() == "print") {
        auto prevExpectedType = expectedType;
        auto &calledArgs = functionCallStatement->getArguments();
        expectedType = std::nullopt;
        for (const auto &arg : calledArgs) {
            arg->accept(*this);
        }
        expectedType = prevExpectedType;
        return;
    }
    if (!symbol.has_value()) {
        throw MyException("Function " + functionCallStatement->getFunctionName() + " not declared", functionCallStatement->getPos());
    }
    if (!symbol.value().isFun()) {
        throw MyException("Cannot call non-function " + functionCallStatement->getFunctionName(), functionCallStatement->getPos());
    }

    auto &calledArgs = functionCallStatement->getArguments();
    auto funArgs = symbol.value().getFuncPointer().value()->getParameters();

    if (calledArgs.size() != funArgs->size()) {
        throw MyException("Function " + functionCallStatement->getFunctionName() + " called with wrong number of arguments", functionCallStatement->getPos());
    }

    for (int i = 0; i < funArgs->size(); i++) {
        auto prevExpectedType = expectedType;
        expectedType = funArgs.value()[i]->getType()->getIdType();
        calledArgs[i]->accept(*this);
        expectedType = prevExpectedType;
    }
}

void SemanticVisitor::visitFunctionDeclaration(Nodes::FunctionDeclaration *functionDeclaration) {
    auto prevExpectedType = expectedType;
    expectedType = functionDeclaration->getReturnType()->getType()->getIdType();

    symbolManager.enterNewContext();
    symbolManager.enterNewScope();
    auto parameters = functionDeclaration->getParameters();
    if (functionDeclaration->getParameters().has_value())
    {
        for (auto & i : parameters.value()) {
            i->accept(*this);
        }

    }

    functionDeclaration->acceptFunctionBody(*this);
    expectedType = prevExpectedType;
    symbolManager.leaveContext();
}

void SemanticVisitor::visitProgram(Nodes::Program *program) {
    if(program->getFunctions().empty())
        throw MyException("Program does not contain any functions!");
    if(program->getFunctions().find("main")==program->getFunctions().end())
        throw MyException("main() function missing!");

    symbolManager.enterNewContext();
    symbolManager.enterNewScope();

    for(const auto & it : program->getVariables())
    {
        it.second->accept(*this);
    }

    for(const auto & it : program->getStructTypes())
    {
        it.second->accept(*this);
    }

    for(const auto & it : program->getVariantTypes())
    {
        it.second->accept(*this);
    }

    for(const auto & it : program->getFunctions())
    {
        auto returnType = it.second->getReturnType()->getType()->getIdType();
        std::variant<IdType, std::string, std::shared_ptr<StructInfo>> typeVariant;
        if (std::holds_alternative<IdType>(returnType)) {
            typeVariant = std::get<IdType>(returnType);
        } else {
            typeVariant = std::get<std::string>(returnType);
        }
        symbolManager.insertSymbol(it.first, SymbolInfo(it.first, typeVariant, true, false, false, it.second.get()));
    }

    for (const auto & it : program->getFunctions()){
        it.second->accept(*this);
    }
    symbolManager.leaveContext();
}

std::string SemanticVisitor::getExpectedTypeAsString() {
    // this should not happen
    if (expectedType == std::nullopt)
        throw MyException("Semantic analyzer Error: Expected type not set!", Position{0, 0});

    if (std::holds_alternative<IdType>(expectedType.value()))
        return Nodes::idTypesToStr[std::get<IdType>(expectedType.value())];
    else
        return std::get<std::string>(expectedType.value());
}

bool SemanticVisitor::doesTypeExist(const std::string &typeName) {
    if (typeName == "INT" || typeName == "FLOAT" || typeName == "BOOLEAN" || typeName == "STR")
        return true;
   /* if (structTypes.find(typeName) != structTypes.end())
        return true;
    if (variantTypes.find(typeName) != variantTypes.end())
        return true;*/
    return false;
}
