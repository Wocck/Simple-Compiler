#include "interpreterVisitor.h"
#include "myException.h"

void InterpreterVisitor::visitBoolLiteral(Nodes::BooleanLiteral *booleanLiteral) { currentValue = booleanLiteral->getValue(); }

void InterpreterVisitor::visitIntLiteral(Nodes::IntLiteral *intLiteral) { currentValue = intLiteral->getValue(); }

void InterpreterVisitor::visitFloatLiteral(Nodes::FloatLiteral *floatLiteral) { currentValue = floatLiteral->getValue(); }

void InterpreterVisitor::visitStringLiteral(Nodes::StringLiteral *stringLiteral) { currentValue = stringLiteral->getValue(); }

void InterpreterVisitor::visitIdentifier(Nodes::Identifier *identifier) {}
void InterpreterVisitor::visitRelOp(Nodes::RelOp *relOp) {}
void InterpreterVisitor::visitArtmOp(Nodes::ArtmOp *artmOp) {}
void InterpreterVisitor::visitFactorOp(Nodes::FactorOp *factorOp) {}
void InterpreterVisitor::visitUnaryOp(Nodes::UnaryOp *unaryOp) {}
void InterpreterVisitor::visitCastOp(Nodes::CastOp *castOp) {}
void InterpreterVisitor::visitType(Nodes::Type *type) {}
void InterpreterVisitor::visitTypeDecl(Nodes::TypeDecl *typeDecl) {}

void InterpreterVisitor::visitCastingExpr(Nodes::CastingExpr *castingExpr) {
    castingExpr->acceptExpr(*this);
    if (castingExpr->getCastOp()) {
        auto op = castingExpr->getCastOp()->getType();
        switch (op) {
            case IdType::INT:
                if (std::holds_alternative<float>(currentValue))
                    currentValue.emplace<int>(static_cast<int>(std::get<float>(currentValue)));
                else
                    throw MyException("Invalid type of argument in casting expr", castingExpr->getPos());
                break;
            case IdType::FLOAT:
                if (std::holds_alternative<int>(currentValue))
                    currentValue.emplace<float>(static_cast<float>(std::get<int>(currentValue)));
                else
                    throw MyException("Invalid type of argument in casting expr", castingExpr->getPos());
                break;
            case IdType::STR:
                if (std::holds_alternative<int>(currentValue))
                    currentValue.emplace<std::string>(std::to_string(std::get<int>(currentValue)));
                else if (std::holds_alternative<float>(currentValue))
                    currentValue.emplace<std::string>(std::to_string(std::get<float>(currentValue)));
                else
                    throw MyException("Invalid type of argument in casting expr", castingExpr->getPos());
                break;
            case IdType::BOOLEAN:
                if (std::holds_alternative<int>(currentValue))
                    currentValue.emplace<bool>(static_cast<bool>(std::get<int>(currentValue)));
                else if (std::holds_alternative<float>(currentValue))
                    currentValue.emplace<bool>(static_cast<bool>(std::get<float>(currentValue)));
                else
                    throw MyException("Invalid type of argument in casting expr", castingExpr->getPos());
                break;
        }
    }
}

void InterpreterVisitor::visitUnaryExpr(Nodes::UnaryExpr *unaryExpr) {
    unaryExpr->acceptExpr(*this);
    if (unaryExpr->getUnaryOp()) {
        auto op = unaryExpr->getUnaryOp()->getType();

        switch (op) {
            case UnaryOperator::NEGATIVE:
                if (std::holds_alternative<int>(currentValue))
                    currentValue.emplace<int>(-std::get<int>(currentValue));
                else if (std::holds_alternative<float>(currentValue))
                    currentValue.emplace<float>(-std::get<float>(currentValue));
                else
                    throw MyException("Invalid type of argument in unary expr", unaryExpr->getPos());
                break;
            case UnaryOperator::NEGATE:
                if (std::holds_alternative<bool>(currentValue))
                    currentValue.emplace<bool>(!std::get<bool>(currentValue));
                else
                    throw MyException("Invalid type of argument in unary expr", unaryExpr->getPos());
                break;
        }
    }
}

void InterpreterVisitor::visitMulExpr(Nodes::MulExpr *mulExpr) {
    mulExpr->acceptLeft(*this);
    auto left = currentValue;

    if (mulExpr->getRightOperand())
    {
        auto op = mulExpr->getFactorOp()->getType();
        mulExpr->acceptRight(*this);

        switch (op) {
            case FactorOperator::MULTIPLY:
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(currentValue))
                    currentValue.emplace<int>(std::get<int>(left) * std::get<int>(currentValue));
                else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(currentValue))
                    currentValue.emplace<float>(std::get<float>(left) * std::get<float>(currentValue));
                else
                    throw MyException("Invalid type of argument in mul expr", mulExpr->getPos());
                break;
            case FactorOperator::DIVIDE:
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(currentValue))
                    currentValue.emplace<int>(std::get<int>(left) / std::get<int>(currentValue));
                else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(currentValue))
                    currentValue.emplace<float>(std::get<float>(left) / std::get<float>(currentValue));
                else
                    throw MyException("Invalid type of argument in mul expr", mulExpr->getPos());
                break;
        }
    }
}

void InterpreterVisitor::visitArtmExpr(Nodes::ArtmExpr *artmExpr) {
    artmExpr->acceptLeft(*this);
    auto left = currentValue;

    if (artmExpr->getRightOperand())
    {
        auto op = artmExpr->getArtmOp()->getType();
        artmExpr->acceptRight(*this);

        switch (op) {
            case ArtmOperator::PLUS:
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(currentValue))
                    currentValue.emplace<int>(std::get<int>(left) + std::get<int>(currentValue));
                else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(currentValue))
                    currentValue.emplace<float>(std::get<float>(left) + std::get<float>(currentValue));
                else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(currentValue))
                    currentValue.emplace<std::string>(std::get<std::string>(left) + std::get<std::string>(currentValue));
                else
                    throw MyException("Invalid type of argument in artm expr", artmExpr->getPos());
                break;
            case ArtmOperator::MINUS:
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(currentValue))
                    currentValue.emplace<int>(std::get<int>(left) - std::get<int>(currentValue));
                else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(currentValue))
                    currentValue.emplace<float>(std::get<float>(left) - std::get<float>(currentValue));
                else
                    throw MyException("Invalid type of argument in artm expr", artmExpr->getPos());
                break;
        }
    }
}

void InterpreterVisitor::visitRelExpr(Nodes::RelExpr *relExpr) {
    relExpr->acceptLeft(*this);
    auto left = currentValue;

    if (relExpr->getRightOperand())
    {
        auto op = relExpr->getRelOp()->getType();
        relExpr->acceptRight(*this);

        switch (op) {
            case RelationalOperator::EQUAL:
                currentValue.emplace<bool>(left == currentValue);
                break;
            case RelationalOperator::NOT_EQUAL:
                currentValue.emplace<bool>(left != currentValue);
                break;
            case RelationalOperator::LESS:
                currentValue.emplace<bool>(left < currentValue);
                break;
            case RelationalOperator::LESS_EQUAL:
                currentValue.emplace<bool>( left <= currentValue);
                break;
            case RelationalOperator::GREATER:
                currentValue.emplace<bool>(left > currentValue);
                break;
            case RelationalOperator::GREATER_EQUAL:
                currentValue.emplace<bool>(left >= currentValue);
                break;
        }
    }
}

void InterpreterVisitor::visitAndExpr(Nodes::AndExpr *andExpr) {
    andExpr->acceptLeft(*this);
    auto left = currentValue;

    if (andExpr->getRightOperand())
    {
        andExpr->acceptRight(*this);
        auto right = currentValue;

        currentValue = valueToBool(left) && valueToBool(right);
    }
}

void InterpreterVisitor::visitOrExpr(Nodes::OrExpr *orExpr) {
    orExpr->acceptLeft(*this);
    auto left = currentValue;

    if (orExpr->getRightOperand())
    {
        orExpr->acceptRight(*this);
        auto right = currentValue;

        currentValue = valueToBool(left) || valueToBool(right);
    }


}

void InterpreterVisitor::visitExpr(Nodes::Expression *expression) {
    auto prevType = expectedType;
    expectedType=std::nullopt;
    expression->acceptExpr(*this);
    expectedType = prevType;
}

void InterpreterVisitor::visitFuncCall(Nodes::FunCall *funCall) {
    auto symbol = symbolManager.getSymbol(funCall->getIdentifier(), true);
    auto funArgs = funCall->getArguments();
    arguments.clear();
    if (funArgs.has_value()) {
        for (auto &arg : funArgs.value()) {
            arg->accept(*this);
            arguments.push_back(currentValue);
        }
    }
    auto func = symbol.value().getFuncPointer().value();
    func->accept(*this);
}

void InterpreterVisitor::visitVariableRef(Nodes::VarReference *varReference) {
    auto symbol = symbolManager.getSymbol(varReference->getIdentifier(), false);

    if(symbol.has_value())
    {
        currentValue = symbol.value().getValue().value();
    }
    else
    {
        auto type = symbol.value().getType();
        if (std::holds_alternative<IdType>(type)) {
            switch (std::get<IdType>(type)) {
                case IdType::INT:
                    currentValue.emplace<int>(0);
                    break;
                case IdType::FLOAT:
                    currentValue.emplace<float>(0.0);
                    break;
                case IdType::BOOLEAN:
                    currentValue.emplace<bool>(false);
                    break;
                case IdType::STR:
                    currentValue.emplace<std::string>("");
                    break;
                default:
                    throw MyException("Invalid type of argument in var reference", varReference->getPos());
            }
        } else {
            currentValue = "!!!!";
        }
    }
}

void InterpreterVisitor::visitDeclaration(Nodes::Declaration *declaration) {}

void InterpreterVisitor::visitVariableDeclaration(Nodes::VariableDeclaration *variableDeclaration) {
    std::variant<int, float, bool, std::string> value;
    auto varType = std::get<IdType>(variableDeclaration->getTypeDecl()->getType()->getIdType());
    switch (varType){
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
        default:
            throw MyException("Invalid type of argument in var declaration", variableDeclaration->getPos());
    }
    symbolManager.insertSymbol(variableDeclaration->getIdentifier(), SymbolInfo(variableDeclaration->getIdentifier(), varType, false, variableDeclaration->isMutable(), true, std::nullopt));

    if (variableDeclaration->getInitExpr()) {
        auto prevExpectedType = expectedType;
        expectedType = varType;
        variableDeclaration->acceptInitExpr(*this);
        symbolManager.setValue(variableDeclaration->getIdentifier(), currentValue);
        expectedType = prevExpectedType;
    }
}

void InterpreterVisitor::visitStructTypeDefinition(Nodes::StructTypeDefinition *structTypeDefinition) {}

void InterpreterVisitor::visitStructVarDeclaration(Nodes::StructVarDeclaration *structVarDeclaration) {
    std::string varName = structVarDeclaration->getIdentifier();
    std::string structTypeName = structVarDeclaration->getTypeName();
    bool isMutable = structVarDeclaration->isMutable();

    auto structValues = structVarDeclaration->getArgs();
    auto &structAgrTypes = structTypes.at(structTypeName)->getFields();
    std::map<std::string, StructFieldInfo> structInstanceFields;
    for (int i = 0; i < structValues.size(); i++) {
        auto fieldType = std::get<IdType>(structAgrTypes[i]->getType()->getIdType());
        auto prevExpectedType = expectedType;
        expectedType = fieldType;
        structValues[i]->accept(*this);
        structInstanceFields.insert(std::make_pair(structAgrTypes[i]->getIdentifier(), StructFieldInfo(fieldType, currentValue)));
        expectedType = prevExpectedType;
    }
    auto structInfo = std::make_shared<StructInfo>(structInstanceFields);
    SymbolInfo structVarSymbol(varName, structInfo, false, isMutable, false, std::nullopt);
    symbolManager.insertSymbol(varName, structVarSymbol);
}


std::variant<int, float, bool, std::string> getDefaultValue(IdType type) {
    switch (type) {
        case IdType::INT:
            return 0;
        case IdType::FLOAT:
            return 0.0f;
        case IdType::BOOLEAN:
            return false;
        case IdType::STR:
            return std::string("");
        default:
            throw std::runtime_error("Invalid type");
    }
}


void InterpreterVisitor::visitVariantTypeDefinition(Nodes::VariantTypeDefinition *variantTypeDefinition) {}
void InterpreterVisitor::visitVariantVarDeclaration(Nodes::VariantVarDeclaration *variantVarDeclaration) {
    std::string varName = variantVarDeclaration->getIdentifier();
    std::string variantTypeName = variantVarDeclaration->getTypeName();
    auto &variantArgs = variantTypes.at(variantTypeName)->getFields();
    auto value = variantVarDeclaration->getValue();
    if (value) {
        value->acceptExpr(*this);
    }
    SymbolInfo variantVarSymbol(varName, variantTypeName, false, true, false, currentValue);
    symbolManager.insertSymbol(varName, variantVarSymbol);
}

void InterpreterVisitor::visitAssignment(Nodes::Assignment *assignment) {
    if(returned)
        return;

    auto symbol = symbolManager.getSymbol(assignment->getIdentifier(), false);
    auto prevExpectedType = expectedType;
    auto symbval = symbol.value();
    //expectedType = symbol.value().getType();
    expectedType = symbval.getType();

    assignment->acceptExpr(*this);
    symbol.value().setValue(currentValue);
    symbolManager.setValue(assignment->getIdentifier(), currentValue);

    if (symbolManager.isGlobal(assignment->getIdentifier()))
        variables[assignment->getIdentifier()] = currentValue;

    expectedType = prevExpectedType;
}

void InterpreterVisitor::visitStructFieldAssignment(Nodes::StructFieldAssignment *structFieldAssignment) {}

void InterpreterVisitor::visitReturnStatement(Nodes::ReturnStatement *returnStatement) {
    if(returned)
        return;
    returnStatement->acceptReturnExpr(*this);
    returned = true;
}

void InterpreterVisitor::visitBlock(Nodes::Block *block) {
    symbolManager.enterNewScope();
    block->acceptStatements(*this);
    symbolManager.leaveScope();
}

void InterpreterVisitor::visitIfStatement(Nodes::IfStatement *ifStatement) {
    if(returned)
        return;

    auto prevExpectedType = expectedType;
    expectedType = std::nullopt;
    ifStatement->acceptCondition(*this);

    if(std::holds_alternative<bool>(currentValue)) {
        auto cond = std::get<bool>(currentValue);
        if(cond)
            ifStatement->acceptIfBlock(*this);
        if(ifStatement->getElseBlock() && cond == 0)
            ifStatement->acceptElseBlock(*this);
    }
    else if (std::holds_alternative<int>(currentValue)) {
        auto cond = std::get<int>(currentValue);
        if(cond)
            ifStatement->acceptIfBlock(*this);
        if(ifStatement->getElseBlock() && cond == 0)
            ifStatement->acceptElseBlock(*this);
    }
    else if (std::holds_alternative<float>(currentValue)) {
        auto cond = std::get<float>(currentValue);
        if(cond != 0.0)
            ifStatement->acceptIfBlock(*this);
        if(ifStatement->getElseBlock() && cond == 0.0)
            ifStatement->acceptElseBlock(*this);
    }
    else // string
        throw MyException("Invalid type of condition", ifStatement->getPos());

    expectedType = prevExpectedType;
}

void InterpreterVisitor::visitWhileStatement(Nodes::WhileStatement *whileStatement) {
    if(returned)
        return;

    auto prevExpectedType = expectedType;
    expectedType = std::nullopt;
    whileStatement->acceptCondition(*this);

    if (std::holds_alternative<bool>(currentValue)){
        bool cond = std::get<bool>(currentValue);
        while (cond){
            whileStatement->acceptWhileBlock(*this);
            whileStatement->acceptCondition(*this);
            cond = std::get<bool>(currentValue);
        }
    }
    else if (std::holds_alternative<int>(currentValue)){
        int cond = std::get<int>(currentValue);
        while (cond){
            whileStatement->acceptWhileBlock(*this);
            whileStatement->acceptCondition(*this);
            cond = std::get<int>(currentValue);
        }
    }
    else if (std::holds_alternative<float>(currentValue)){
        float cond = std::get<float>(currentValue);
        while (cond){
            whileStatement->acceptWhileBlock(*this);
            whileStatement->acceptCondition(*this);
            cond = std::get<float>(currentValue);
        }
    }
    else // string
        throw MyException("Invalid type of condition", whileStatement->getPos());
    expectedType = prevExpectedType;
}

void InterpreterVisitor::visitFunctionCallStatement(Nodes::FunctionCallStatement *functionCallStatement) {
    if(returned)
        return;
    auto symbol =symbolManager.getSymbol(functionCallStatement->getFunctionName(), true);
    if (functionCallStatement->getFunctionName() == "print") {
        if (!functionCallStatement->getArguments().empty()) {
            for (auto &arg: functionCallStatement->getArguments()) {
                arg->accept(*this);
                if (std::holds_alternative<int>(currentValue))
                    std::cout << std::get<int>(currentValue);
                else if (std::holds_alternative<float>(currentValue))
                    std::cout << std::get<float>(currentValue);
                else if (std::holds_alternative<bool>(currentValue))
                    std::cout << std::get<bool>(currentValue);
                else if (std::holds_alternative<std::string>(currentValue))
                    std::cout << std::get<std::string>(currentValue);
                else
                    throw MyException("Invalid type of argument in function call stmt", functionCallStatement->getPos());
            }
        } else
            std::cout << std::endl;
        return;
    }

    auto &calledArgs = functionCallStatement->getArguments();
    arguments.clear();
    for (auto &arg : calledArgs) {
        arg->accept(*this);
        arguments.push_back(currentValue);
    }

    symbol.value().getFuncPointer().value()->accept(*this);
}

void InterpreterVisitor::visitFunctionDeclaration(Nodes::FunctionDeclaration *functionDeclaration) {
    auto funReturnType = functionDeclaration->getReturnType()->getType()->getIdType();
    std::variant<IdType, std::string, std::shared_ptr<StructInfo>> typeVariant;
    if (std::holds_alternative<IdType>(funReturnType)) {
        typeVariant = std::get<IdType>(funReturnType);
    } else {
        typeVariant = std::get<std::string>(funReturnType);
    }
    symbolManager.insertSymbol(functionDeclaration->getFunctionName(), SymbolInfo(functionDeclaration->getFunctionName(), typeVariant, true, false, false, functionDeclaration));

    auto prevType = expectedType;
    expectedType = functionDeclaration->getReturnType()->getType()->getIdType();
    symbolManager.enterNewContext();
    symbolManager.enterNewScope();

    if (functionDeclaration->getParameters().has_value())
    {
        auto parameters = functionDeclaration->getParameters().value();
        for (int i = 0; i < parameters.size(); i++)
        {
            auto &param = parameters[i];
            auto paramType = param->getType()->getIdType();
            std::variant<IdType, std::string, std::shared_ptr<StructInfo>> type;
            if (std::holds_alternative<IdType>(paramType)) {
                type = std::get<IdType>(paramType);
            } else {
                type = std::get<std::string>(paramType);
            }
            symbolManager.insertSymbol(param->getIdentifier(), SymbolInfo(param->getIdentifier(), type, false, true, false, arguments[i]));
        }
    }

    functionDeclaration->acceptFunctionBody(*this);
    if (returned)
        returned = false;

    symbolManager.leaveContext();
    expectedType = prevType;
}

void InterpreterVisitor::visitProgram(Nodes::Program *program) {
    symbolManager.enterNewContext();
    symbolManager.enterNewScope();

    for (const auto& var : program->getVariables()){
        var.second->accept(*this);
    }

    for (const auto& func : program->getFunctions()){
        auto returnType = func.second->getReturnType()->getType()->getIdType();
        std::variant<IdType, std::string, std::shared_ptr<StructInfo>> typeVariant;
        if (std::holds_alternative<IdType>(returnType)) {
            typeVariant = std::get<IdType>(returnType);
        } else {
            typeVariant = std::get<std::string>(returnType);
        }
        symbolManager.insertSymbol(func.first, SymbolInfo(func.first, typeVariant, true, false, false, func.second.get()));
    }

    program->getFunctions().find("main")->second->accept(*this);
    symbolManager.leaveContext();
}

bool InterpreterVisitor::valueToBool(std::variant<int, float, bool, std::string> variant) {
    return std::visit([](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, bool>) {
            return value;
        } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
            return value != 0; // 0 lub 0.0 to fałsz, wszystko inne to prawda
        } else {
            throw std::runtime_error("Invalid type for logical operation");
        }
    }, variant);
}
