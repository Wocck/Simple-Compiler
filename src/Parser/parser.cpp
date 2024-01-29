#include "parser.h"

// *********************************************************************************************************************
//                         Helper functions
// *********************************************************************************************************************
bool Parser::matchToken(TokenTypes tokenType) const {
    return currToken.getType() == tokenType;
}

void Parser::getNextToken() {
    currToken = lexer.getNextToken();
    while(currToken.getType() == TokenTypes::SINGLE_COMMENT || currToken.getType() == TokenTypes::MULTILINE_COMMENT_START) {
        currToken = lexer.getNextToken();
    }
    if(currToken.getType() == TokenTypes::UNDEF)
        throw MyException("Undefined token!", currToken.getPosition());
}

void Parser::consumeToken(TokenTypes tokenType, const std::string &exceptionMessage) {
    if(!matchToken(tokenType)) {
        throw MyException(exceptionMessage, currToken.getPosition());
    }
    getNextToken();
}

bool Parser::isIdType(TokenTypes type) const {
    if (type == TokenTypes::INT_KW ||
        type == TokenTypes::FLOAT_KW ||
        type == TokenTypes::STR_KW ||
        type == TokenTypes::BOOL_KW ||
        type == TokenTypes::STRUCT_KW ||
        type == TokenTypes::VARIANT_KW )
        return true;
    return false;
}

bool Parser::isSimpleVarType(TokenTypes type) const {
    if (type == TokenTypes::INT_KW ||
        type == TokenTypes::FLOAT_KW ||
        type == TokenTypes::STR_KW ||
        type == TokenTypes::BOOL_KW)
        return true;
    return false;
}

IdType Parser::getIdTypeOfToken(TokenTypes) const {
    if (currToken.getType() == TokenTypes::INT_KW) return IdType::INT;
    if (currToken.getType() == TokenTypes::FLOAT_KW) return IdType::FLOAT;
    if (currToken.getType() == TokenTypes::STR_KW) return IdType::STR;
    if (currToken.getType() == TokenTypes::BOOL_KW) return IdType::BOOLEAN;
    if (currToken.getType() == TokenTypes::STRUCT_KW) return IdType::STRUCT;
    if (currToken.getType() == TokenTypes::VARIANT_KW) return IdType::VARIANT;
    throw MyException("Errow in getIdTypeOfToken (this should never be thrown)", currToken.getPosition());
}

bool Parser::contains(const std::vector<std::string> &vec, const std::string &value) const {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

bool Parser::checkIfStructTypeExists(const std::string &id) const {
    auto sType = structTypes.find(id);
    if (sType != structTypes.end())
        return true;
    return false;
    //throw MyException("Type with this id already exists", currToken.getPosition());
}

bool Parser::checkIfVariantTypeExists(const std::string& id) const {
    auto vType = variantTypes.find(id);
    if (vType != variantTypes.end())
        return true;
    return false;
    //throw MyException("Type with this id already exists", currToken.getPosition());
}

bool Parser::checkIfFunctionExists(const std::string &id) const {
    auto fun = functions.find(id);
    if (fun != functions.end())
        return false;
        // throw MyException("Function with this id already exists", currToken.getPosition());
    return true;
}

bool Parser::checkIfVariableExists(const std::string &id) const {
    auto var =variables.find(id);
    if (var != variables.end())
        return false;
        //throw MyException("Variable with this id already exists", currToken.getPosition());
    return true;
}

// *********************************************************************************************************************
//                  Literals Parsing
// *********************************************************************************************************************

std::unique_ptr<Nodes::StringLiteral> Parser::parseStringLiteral() {
    if (currToken.getType() == TokenTypes::STR_VALUE){
        std::string value = std::get<std::string>(currToken.getValue());
        getNextToken();
        return std::make_unique<Nodes::StringLiteral>(value, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::FloatLiteral> Parser::parseFloatLiteral() {
    if (currToken.getType() == TokenTypes::FLOAT_VALUE){
        float value = std::get<float>(currToken.getValue());
        getNextToken();
        return std::make_unique<Nodes::FloatLiteral>(value, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::IntLiteral> Parser::parseIntLiteral() {
    if (currToken.getType() == TokenTypes::INT_VALUE){
        int value = std::get<int>(currToken.getValue());
        getNextToken();
        return std::make_unique<Nodes::IntLiteral>(value, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::BooleanLiteral> Parser::parseBooleanLiteral() {
    if (currToken.getType() == TokenTypes::TRUE_KW){
        bool value = true;
        getNextToken();
        return std::make_unique<Nodes::BooleanLiteral>(value, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::FALSE_KW){
        bool value = false;
        getNextToken();
        return std::make_unique<Nodes::BooleanLiteral>(value, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::Identifier> Parser::parseIdentifier() {
    if (currToken.getType() == TokenTypes::IDENTIFIER){
        std::string value = std::get<std::string>(currToken.getValue());
        getNextToken();
        return std::make_unique<Nodes::Identifier>(value, currToken.getPosition());
    }
    return nullptr;
}

// *********************************************************************************************************************
//                  Operators Parsing
// *********************************************************************************************************************

std::unique_ptr<Nodes::RelOp> Parser::parseRelOp() {
    if (currToken.getType() == TokenTypes::LESS){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(LESS, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::LESS_EQUAL){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(LESS_EQUAL, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::GREATER){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(GREATER, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::GREATER_EQUAL){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(GREATER_EQUAL, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::EQUAL){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(EQUAL, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::NOT_EQUAL){
        getNextToken();
        return std::make_unique<Nodes::RelOp>(NOT_EQUAL, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::ArtmOp> Parser::parseArtmOp() {
    if (currToken.getType() == TokenTypes::PLUS) {
        getNextToken();
        return std::make_unique<Nodes::ArtmOp>(PLUS, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::MINUS) {
        getNextToken();
        return std::make_unique<Nodes::ArtmOp>(MINUS, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::FactorOp> Parser::parseFactorOp() {
    if (currToken.getType() == TokenTypes::MULTIPLY) {
        getNextToken();
        return std::make_unique<Nodes::FactorOp>(MULTIPLY, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::DIVIDE) {
        getNextToken();
        return std::make_unique<Nodes::FactorOp>(DIVIDE, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::UnaryOp> Parser::parseUnaryOp() {
    if (currToken.getType() == TokenTypes::NEGATE) {
        getNextToken();
        return std::make_unique<Nodes::UnaryOp>(NEGATE, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::MINUS) {
        getNextToken();
        return std::make_unique<Nodes::UnaryOp>(NEGATIVE, currToken.getPosition());
    }
    return nullptr;
}

std::unique_ptr<Nodes::CastOp> Parser::parseCastOp(){
    if (currToken.getType() == TokenTypes::INT_KW){
        getNextToken();
        return std::make_unique<Nodes::CastOp>(INT, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::FLOAT_KW){
        getNextToken();
        return std::make_unique<Nodes::CastOp>(FLOAT, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::STR_KW){
        getNextToken();
        return std::make_unique<Nodes::CastOp>(STR, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::BOOL_KW){
        getNextToken();
        return std::make_unique<Nodes::CastOp>(BOOLEAN, currToken.getPosition());
    }
    return nullptr;
}

// *********************************************************************************************************************
//                  Expressions Parsing
// *********************************************************************************************************************

// num or fun(1, 2, 3)
std::unique_ptr<Nodes::Factor> Parser::parseFunctionCallOrVarRef() {
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        return nullptr;
    std::string identifier = std::get<std::string>(currToken.getValue());
    Position pos = currToken.getPosition();
    getNextToken();
    if (currToken.getType() != TokenTypes::PAREN_LEFT)
        return std::make_unique<Nodes::VarReference>(identifier, pos);
    getNextToken();
    auto arguments = parseArguments();
    if (currToken.getType() != TokenTypes::PAREN_RIGHT)
        throw MyException("Expected ')' after function call", currToken.getPosition());
    getNextToken();
    return std::make_unique<Nodes::FunCall>(identifier, std::move(arguments), currToken.getPosition());
}

std::unique_ptr<Nodes::CastingExpr> Parser::parseCastingExpression() {
    auto expr = parseFactor();
    if (!expr)
        return nullptr;
    if (currToken.getType() == TokenTypes::AS_KW) {
        getNextToken();
        if (currToken.getType() != TokenTypes::BRACKET_LEFT)
            throw MyException("Expected '(' after 'as'", currToken.getPosition());
        getNextToken();
        auto castOp = parseCastOp();
        if (castOp == nullptr)
            throw MyException("Invalid type in casting expression", currToken.getPosition());
        if (currToken.getType() != TokenTypes::BRACKET_RIGHT)
            throw MyException("Expected ']' after type in casting expression", currToken.getPosition());
        getNextToken();
        return std::make_unique<Nodes::CastingExpr>(std::move(expr), std::move(castOp), currToken.getPosition());
    }
    return std::make_unique<Nodes::CastingExpr>(std::move(expr),  currToken.getPosition());
}

std::unique_ptr<Nodes::UnaryExpr> Parser::parseUnaryExpression() {
    if (currToken.getType() == TokenTypes::NEGATE || currToken.getType() == TokenTypes::MINUS){
        auto op = parseUnaryOp();
        auto expr = parseCastingExpression();
        if (!expr)
            throw MyException("Expected expression after unary operator", currToken.getPosition());
        return std::make_unique<Nodes::UnaryExpr>(std::move(op), std::move(expr), currToken.getPosition());
    }
    auto expr = parseCastingExpression();
    if (!expr)
        return nullptr;
    return std::make_unique<Nodes::UnaryExpr>( std::move(expr), currToken.getPosition());
}

std::unique_ptr<Nodes::MulExpr> Parser::parseMulExpression() {
    auto leftExpr = parseUnaryExpression();
    if (!leftExpr)
        return nullptr;
    auto op = parseFactorOp();
    if(!op)
        return std::make_unique<Nodes::MulExpr>(std::move(leftExpr), currToken.getPosition());
    auto rightExpr = parseMulExpression();
    if (rightExpr && !op) // possibly smth - smth
        throw MyException("Missing operator between factors", currToken.getPosition());
    if (!rightExpr && op)
        throw MyException("Missing right factor (consider using parentheses)", currToken.getPosition());
    if (!rightExpr && !op)
        return std::make_unique<Nodes::MulExpr>(std::move(leftExpr), currToken.getPosition());
    return std::make_unique<Nodes::MulExpr>(std::move(leftExpr), std::move(op), std::move(rightExpr), currToken.getPosition());
}

std::unique_ptr<Nodes::ArtmExpr> Parser::parseArtmExpression() {
    auto leftExpr = parseMulExpression();
    if (!leftExpr)
        return nullptr;
    auto op = parseArtmOp();
    auto rightExpr = parseArtmExpression();
    if (rightExpr && !op)
        throw MyException("Missing operator between factors", currToken.getPosition());
    if (!rightExpr && op)
        throw MyException("Missing right factor (consider using parentheses)", currToken.getPosition());
    if (!rightExpr && !op)
        return std::make_unique<Nodes::ArtmExpr>(std::move(leftExpr), currToken.getPosition());
    return std::make_unique<Nodes::ArtmExpr>(std::move(leftExpr), std::move(op), std::move(rightExpr), currToken.getPosition());
}

std::unique_ptr<Nodes::RelExpr> Parser::parseRelExpression() {
    auto leftExpr = parseArtmExpression();
    if (!leftExpr)
        return nullptr;
    auto op = parseRelOp();
    auto rightExpr = parseRelExpression();
    if (rightExpr && !op)
        throw MyException("Missing operator between factors", currToken.getPosition());
    if (!rightExpr && op)
        throw MyException("Missing right factor (consider using parentheses)", currToken.getPosition());
    if (!rightExpr && !op)
        return std::make_unique<Nodes::RelExpr>(std::move(leftExpr), currToken.getPosition());
    return std::make_unique<Nodes::RelExpr>(std::move(leftExpr), std::move(op), std::move(rightExpr), currToken.getPosition());
}

std::unique_ptr<Nodes::AndExpr> Parser::parseAndExpression() {
    auto leftExpr = parseRelExpression();
    if (!leftExpr)
        return nullptr;
    if (currToken.getType() == TokenTypes::AND) {
        getNextToken(); // Consume the AND token
        auto rightExpr = parseAndExpression(); // Recursive call
        if (!rightExpr) {
            throw MyException("Missing right factor (consider using parentheses)", currToken.getPosition());
        }
        return std::make_unique<Nodes::AndExpr>(std::move(leftExpr), std::move(rightExpr), currToken.getPosition());
    }
    return std::make_unique<Nodes::AndExpr>(std::move(leftExpr),  currToken.getPosition());
}

std::unique_ptr<Nodes::OrExpr> Parser::parseOrExpression() {
    auto leftExpr = parseAndExpression();
    if (!leftExpr)
        return nullptr;
    if (currToken.getType() == TokenTypes::OR) {
        getNextToken(); // Consume the OR token
        auto rightExpr = parseOrExpression(); // Recursive call
        if (!rightExpr) {
            throw MyException("Missing right factor (consider using parentheses)", currToken.getPosition());
        }
        return std::make_unique<Nodes::OrExpr>(std::move(leftExpr), std::move(rightExpr), currToken.getPosition());
    }
    return std::make_unique<Nodes::OrExpr>(std::move(leftExpr), currToken.getPosition());
}

std::unique_ptr<Nodes::Expression> Parser::parseExpression() {
    auto expr = parseOrExpression();
    if (expr == nullptr)
        return nullptr;
    return std::make_unique<Nodes::Expression>(std::move(expr), currToken.getPosition());
}

// (expr) or 123 or 1.25 or "string" or True or False or fun(1, 2, 3)
std::unique_ptr<Nodes::Factor> Parser::parseFactor() {
    if (currToken.getType() == TokenTypes::PAREN_LEFT){
        getNextToken();
        auto expr = parseOrExpression();
        if (!expr)
            throw MyException("Invalid expression in parentheses", currToken.getPosition());
        consumeToken(TokenTypes::PAREN_RIGHT, "Expected ')' after expression in parentheses");
        return expr;
    }

    if (auto intNumber = parseIntLiteral())
        return intNumber;
    if (auto floatNumber = parseFloatLiteral())
        return floatNumber;
    if (auto string = parseStringLiteral())
        return string;
    if (auto boolean = parseBooleanLiteral())
        return boolean;
    if (auto funCall = parseFunctionCallOrVarRef())
        return funCall;

    return nullptr;
}

// *********************************************************************************************************************
//                  Statements Parsing
// *********************************************************************************************************************

std::unique_ptr<Nodes::FunctionDeclaration> Parser::parseFunctionDeclaration() {
    if (currToken.getType() != TokenTypes::FUN_KW)
        return nullptr;
    getNextToken();
    std::unique_ptr<Nodes::TypeDecl> funTypeDecl = parseTypeDeclaration();
    if (!funTypeDecl)
        throw MyException("Invalid type declaration in function declaration", currToken.getPosition());
    consumeToken(TokenTypes::PAREN_LEFT, "Expected '(' after function name");
    std::vector<std::unique_ptr<Nodes::TypeDecl>> types;

    while(currToken.getType() != TokenTypes::PAREN_RIGHT){
        auto typeDecl = parseTypeDeclaration();
        if (!typeDecl)
            throw MyException("Invalid type declaration in function declaration", currToken.getPosition());
        types.push_back(std::move(typeDecl));
        if (currToken.getType() != TokenTypes::PAREN_RIGHT) {
            consumeToken(TokenTypes::COMMA, "Expected ',' after type declaration in function declaration");
            if (currToken.getType() == TokenTypes::PAREN_RIGHT)
                throw MyException("Expected type declaration after ',' in function declaration", currToken.getPosition());
        }
    }
    getNextToken(); // skoro wyszliśmy z while
    std::unique_ptr<Nodes::Block> block = parseBlock();
    if (!block)
        throw MyException("Invalid body of function declaration", currToken.getPosition());
    return std::make_unique<Nodes::FunctionDeclaration>(std::move(funTypeDecl), std::move(types), std::move(block), currToken.getPosition());
}

std::unique_ptr<Nodes::WhileStatement> Parser::parseWhileStatement() {
    if (currToken.getType() != TokenTypes::WHILE_KW)
        return nullptr;

    getNextToken();
    auto expression = parseExpression();
    if (!expression)
        throw MyException("Invalid expression in while statement", currToken.getPosition());


    auto whileBlock = parseBlock();
    if (!whileBlock)
        throw MyException("Invalid body of while statement", currToken.getPosition());


    return std::make_unique<Nodes::WhileStatement>(std::move(expression), std::move(whileBlock), currToken.getPosition());
}

std::unique_ptr<Nodes::IfStatement> Parser::parseIfStatement() {
    if (currToken.getType() != TokenTypes::IF_KW)
        return nullptr;

    getNextToken();
    auto expression = parseExpression();
    if (!expression)
        throw MyException("Invalid expression in if statement", currToken.getPosition());

    auto ifBlock = parseBlock();
    if (!ifBlock)
        throw MyException("Invalid body of if statement", currToken.getPosition());

    std::unique_ptr<Nodes::Block> elseBlock = nullptr;
    if (currToken.getType() == TokenTypes::ELSE_KW){
        getNextToken();
        elseBlock = parseBlock();
        if (!elseBlock)
            throw MyException("No body after else statement", currToken.getPosition());
    }
    return std::make_unique<Nodes::IfStatement>(
            std::move(expression),
            std::move(ifBlock),
            std::move(elseBlock),
            currToken.getPosition());
}

std::unique_ptr<Nodes::Block> Parser::parseBlock() {
    std::set<std::string> declaredIDs;
    consumeToken(TokenTypes::BRACKET_LEFT, "Expected '[' at the beginning of block");
    std::vector<std::unique_ptr<Nodes::Statement>> statements;
    std::unique_ptr<Nodes::Statement> stmt;
    while ( (stmt = parseStatement(declaredIDs)) )
        statements.push_back(std::move(stmt));
    consumeToken(TokenTypes::BRACKET_RIGHT, "Expected ']' at the end of block");
    return std::make_unique<Nodes::Block>(std::move(statements), currToken.getPosition());
}

std::unique_ptr<Nodes::ReturnStatement> Parser::parseReturnStatement() {
    if (currToken.getType() != TokenTypes::RETURN_KW){
        return nullptr;
    }

    getNextToken();
    if (currToken.getType() == TokenTypes::SEMICOLON){
        getNextToken();
        return std::make_unique<Nodes::ReturnStatement>(nullptr, currToken.getPosition());
    }
    auto expression = parseExpression();
    if (expression == nullptr)
        throw MyException("Invalid return statement", currToken.getPosition());
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after expression in return statement");
    return std::make_unique<Nodes::ReturnStatement>(std::move(expression), currToken.getPosition());
}

std::unique_ptr<Nodes::FunctionCallStatement> Parser::parseFunctionCallStatement(std::string identifier){
    getNextToken();
    auto args = parseArguments();
    consumeToken(TokenTypes::PAREN_RIGHT, "Expected ')' after function call");
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after function call");
    return std::make_unique<Nodes::FunctionCallStatement>(std::move(identifier), std::move(args), currToken.getPosition());
}

std::unique_ptr<Nodes::Assignment> Parser::parseAssignment(std::string identifier){
    consumeToken(TokenTypes::ASSIGN,"no assign after id token");
    std::unique_ptr<Nodes::Expression> expr = parseExpression();
    if (!expr)
        throw MyException("Invalid expression in assignment", currToken.getPosition());
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after expression in assignment");
    return std::make_unique<Nodes::Assignment>(std::move(identifier), std::move(expr), currToken.getPosition());
}

std::unique_ptr<Nodes::StructFieldAssignment> Parser::parseStructFieldAssignment(std::string identifier) {
    getNextToken();
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        throw MyException("Expected identifier after '.'", currToken.getPosition());
    std::string field = std::get<std::string>(currToken.getValue());
    getNextToken();
    if (currToken.getType() != TokenTypes::ASSIGN)
        throw MyException("Expected '=' after field name", currToken.getPosition());
    getNextToken();
    std::unique_ptr<Nodes::Expression> expr = parseExpression();
    if (!expr)
        throw MyException("Invalid expression in assignment", currToken.getPosition());
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after expression in assignment");
    return std::make_unique<Nodes::StructFieldAssignment>(std::move(identifier), std::move(field), std::move(expr), currToken.getPosition());
}

std::unique_ptr<Nodes::Statement> Parser::parseAssignmentOrCallOrVar(std::set<std::string>& declaredIds) {
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        return nullptr;
    std::string identifier = std::get<std::string>(currToken.getValue());
    Position typePos = currToken.getPosition();
    getNextToken();

    // Base cases
    switch(currToken.getType()){
        case TokenTypes::PAREN_LEFT:
            return parseFunctionCallStatement(identifier);
        case TokenTypes::ASSIGN:
            return parseAssignment(identifier);
        case TokenTypes::DOT:
            return parseStructFieldAssignment(identifier);
        default:
            break;
    }

    // to znaczy że doublecollon czyli mamy "id::coś..."
    if (currToken.getType() == TokenTypes::DOUBLE_COLON) {
        bool isMutable = false; // bo nie ma mut_kw
        std::string type = identifier;
        consumeToken(TokenTypes::DOUBLE_COLON, "Expected '::' after type name");
        if (currToken.getType() != TokenTypes::IDENTIFIER)
            throw MyException("Expected identifier after '::'", currToken.getPosition());
        std::string id = std::get<std::string>(currToken.getValue());
        Position idPos = currToken.getPosition();
        if (!declaredIds.insert(id).second)
            throw MyException("redefiniton found in local variable definition", currToken.getPosition());

        // mamy typ (variant albo struct) i id
        getNextToken();
        auto typeNode = std::make_unique<Nodes::Type>(type, typePos);
        auto typeDeclNode = std::make_unique<Nodes::TypeDecl>(std::move(typeNode), id, idPos);
        if (currToken.getType() == TokenTypes::PAREN_LEFT) { // to znaczy że zmienna struktury
            auto structVar = parseStructVarDeclaration(isMutable, std::move(typeDeclNode));
            if (!structVar)
                throw MyException("Invalid struct variable declaration", currToken.getPosition());
            return std::move(structVar);
        }
        if (currToken.getType() == TokenTypes::ASSIGN) { // to znaczy że zmienna typu variant
            auto variantVar = parseVariantVarDeclaration(std::move(typeDeclNode));
            if (!variantVar)
                throw MyException("Invalid variant variable declaration", currToken.getPosition());
            return std::move(variantVar);
        } // to znaczy że pusta zmienna variant
        else{
            consumeToken(TokenTypes::SEMICOLON, "Expected ';' after variant variable declaration");
            return std::make_unique<Nodes::VariantVarDeclaration>(std::move(typeDeclNode), nullptr, currToken.getPosition());
        }
    }
    throw MyException("This should not be reached!", currToken.getPosition());
}



std::unique_ptr<Nodes::Statement> Parser::parseStatement(std::set<std::string>& declaredIds) {
    std::unique_ptr<Nodes::Statement> statement;

    statement = parseIfStatement();
    if(statement)
        return std::move(statement);

    statement = parseWhileStatement();
    if(statement)
        return std::move(statement);

    statement = parseReturnStatement();
    if(statement)
        return std::move(statement);

    statement = parseAssignmentOrCallOrVar(declaredIds);
    if(statement)
        return std::move(statement);

    statement = parseLocalVarDeclaration(declaredIds);
    if(statement)
        return std::move(statement);

    return nullptr;
}

// 1, 2, 3, fun(), param
std::vector<std::unique_ptr<Nodes::Expression>> Parser::parseArguments() {
    std::vector<std::unique_ptr<Nodes::Expression>> args;
    std::unique_ptr<Nodes::Expression> arg = parseExpression();
    if (arg)
        args.push_back(std::move(arg));
    else
        return args;
    while (currToken.getType() == TokenTypes::COMMA){
        getNextToken();
        arg = parseExpression();
        if (arg)
            args.push_back(std::move(arg));
        else
            throw MyException("Invalid expression in argument list", currToken.getPosition());
    }
    return std::move(args);
}

// *********************************************************************************************************************
//                  Declarations Parsing
// *********************************************************************************************************************

std::unique_ptr<Nodes::VariantTypeDefinition> Parser::parseVariantTypeDefinition() {
    if (currToken.getType() != TokenTypes::VARIANT_KW)
        return nullptr;
    getNextToken();
    consumeToken(TokenTypes::DOUBLE_COLON, "Expected '::' after 'variant' keyword");
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        throw MyException("Expected identifier after 'variant' keyword", currToken.getPosition());
    std::string identifier = std::get<std::string>(currToken.getValue());
    getNextToken();
    consumeToken(TokenTypes::PAREN_LEFT, "Expected '(' after identifier in variant type definition");
    std::vector<std::unique_ptr<Nodes::Type>> types;
    while(currToken.getType() != TokenTypes::PAREN_RIGHT){
        auto type = parseType();
        if (!type)
            throw MyException("Invalid type in variant type definition", currToken.getPosition());
        types.push_back(std::move(type));
        consumeToken(TokenTypes::SEMICOLON, "Expected ';' after type in variant type definition");
    }
    getNextToken(); // skoro wyszliśmy z while
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after variant type definition");
    variantTypeNames.push_back(identifier); // trzeba będzie zmienić na symbolTable;
    return std::make_unique<Nodes::VariantTypeDefinition>(identifier, std::move(types), currToken.getPosition());
}

std::unique_ptr<Nodes::VariantVarDeclaration> Parser::parseVariantVarDeclaration(std::unique_ptr<Nodes::TypeDecl> typeDecl) {
    consumeToken(TokenTypes::ASSIGN, "Expected '=' after variant variable name");
    std::unique_ptr<Nodes::Expression> expr = parseExpression();
    if (!expr)
        throw MyException("Invalid expression in variant variable declaration", currToken.getPosition());
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after expression in variant variable declaration");
    return std::make_unique<Nodes::VariantVarDeclaration>(std::move(typeDecl), std::move(expr), currToken.getPosition());
}

std::unique_ptr<Nodes::StructVarDeclaration> Parser::parseStructVarDeclaration(bool isMutable, std::unique_ptr<Nodes::TypeDecl> typeDecl) {
    consumeToken(TokenTypes::PAREN_LEFT, "Expected '(' after struct variable name");
    std::vector<std::unique_ptr<Nodes::Expression>> args;
    while(currToken.getType() != TokenTypes::PAREN_RIGHT){
        auto arg = parseExpression();
        if (!arg)
            throw MyException("Invalid expression in struct variable declaration", currToken.getPosition());
        args.push_back(std::move(arg));
        if (currToken.getType() != TokenTypes::PAREN_RIGHT)
            consumeToken(TokenTypes::COMMA, "Expected ',' after expression in struct variable declaration");
    }
    getNextToken(); // skoro wyszliśmy z while
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after struct variable declaration");
    return std::make_unique<Nodes::StructVarDeclaration>(isMutable, std::move(typeDecl), std::move(args), currToken.getPosition());
}

std::unique_ptr<Nodes::StructTypeDefinition> Parser::parseStructTypeDefinition() {
    if (currToken.getType() != TokenTypes::STRUCT_KW)
        return nullptr;
    getNextToken();
    consumeToken(TokenTypes::DOUBLE_COLON, "Expected '::' after 'struct' keyword");
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        throw MyException("Expected identifier after 'struct' keyword", currToken.getPosition());
    std::string identifier = std::get<std::string>(currToken.getValue());
    getNextToken();
    consumeToken(TokenTypes::PAREN_LEFT, "Expected '(' after identifier in struct type definition");
    std::vector<std::unique_ptr<Nodes::TypeDecl>> types;
    while(currToken.getType() != TokenTypes::PAREN_RIGHT){
        auto typeDecl = parseTypeDeclaration();
        if (!typeDecl)
            throw MyException("Invalid type declaration in struct type definition", currToken.getPosition());
        types.push_back(std::move(typeDecl));
        consumeToken(TokenTypes::SEMICOLON, "Expected ';' after type declaration in struct type definition");
    }
    getNextToken(); // skoro wyszliśmy z while
    consumeToken(TokenTypes::SEMICOLON, "Expected ';' after struct type definition");
    structTypeNames.push_back(identifier);
    return std::make_unique<Nodes::StructTypeDefinition>(identifier, std::move(types), currToken.getPosition());
}

std::unique_ptr<Nodes::VariableDeclaration> Parser::parseSimpleVariableDeclaration(bool isMut) {
    Position varPos = currToken.getPosition();
    auto typeDecl = parseTypeDeclaration();
    if (!typeDecl)
        return nullptr;
    std::unique_ptr<Nodes::Expression> initExpr;
    if (currToken.getType() != TokenTypes::ASSIGN && currToken.getType() != TokenTypes::SEMICOLON)
        return nullptr;
    if (currToken.getType() == TokenTypes::SEMICOLON){
        getNextToken();
        return std::make_unique<Nodes::VariableDeclaration>(isMut, std::move(typeDecl), nullptr, currToken.getPosition());
    }
    if (currToken.getType() == TokenTypes::ASSIGN){
        getNextToken();
        initExpr = parseExpression();
        if (!initExpr)
            throw MyException("Invalid expression in variable declaration", currToken.getPosition());
        if (currToken.getType() != TokenTypes::SEMICOLON)
            throw MyException("Expected ';' after expression in variable declaration", currToken.getPosition());
        getNextToken();
        return std::make_unique<Nodes::VariableDeclaration>(isMut, std::move(typeDecl), std::move(initExpr), varPos);
    }
    throw MyException("Invalid variable declaration", currToken.getPosition());
}

// int, float, str, bool, struct, variant or user_type(identifier)
std::unique_ptr<Nodes::Type> Parser::parseType() {
    std::string typeName;
    IdType type;
    if (currToken.getType() == TokenTypes::IDENTIFIER){
        typeName = std::get<std::string>(currToken.getValue());
        getNextToken();
    } else if (isIdType(currToken.getType())){
        type = getIdTypeOfToken(currToken.getType());
        getNextToken();
    } else
        return nullptr;
    if (typeName.empty())
        return std::make_unique<Nodes::Type>(type, currToken.getPosition());
    else
        return std::make_unique<Nodes::Type>(typeName, currToken.getPosition());
}

// variant::ala, int::a, float::b, str::c
std::unique_ptr<Nodes::TypeDecl> Parser::parseTypeDeclaration() {
    Position typePos = currToken.getPosition();
    std::string identifier;
    std::unique_ptr<Nodes::Type> type = parseType();
    if (!type)
        return nullptr;
    if (currToken.getType() != TokenTypes::DOUBLE_COLON)
        return nullptr;
    getNextToken();
    if (currToken.getType() != TokenTypes::IDENTIFIER)
        throw MyException("Expected identifier after '::'", currToken.getPosition());

    identifier = std::get<std::string>(currToken.getValue());
    getNextToken();
    return std::make_unique<Nodes::TypeDecl>(std::move(type), identifier, typePos);
}

std::unique_ptr<Nodes::Declaration> Parser::parseLocalVarDeclaration(std::set<std::string> &declaredIds) {
    bool isMutable = false;
    if (currToken.getType() == TokenTypes::MUT_KW){
        isMutable = true;
        getNextToken();
    }

    // mut int::a; float::b; str::c; mut bool:g;
    if (isSimpleVarType(currToken.getType())) {
        auto varDecl = parseSimpleVariableDeclaration(isMutable);
        std::string id = varDecl->getIdentifier();
        if (!varDecl)
            throw MyException("Invalid variable declaration", currToken.getPosition());
        if(!declaredIds.insert(id).second)
            throw MyException("redefiniton found in local variable definition", currToken.getPosition());
        return std::move(varDecl);
    }

    // struct::pos(int::x);
    if (currToken.getType() == TokenTypes::STRUCT_KW){
        auto structTypeDef = parseStructTypeDefinition();
        std::string id = structTypeDef->getStructName();
        if (!structTypeDef)
            throw MyException("Invalid struct type definition", currToken.getPosition());
        if(!declaredIds.insert(id).second)
            throw MyException("redefiniton found in local struct type definition", currToken.getPosition());
        return std::move(structTypeDef);
    }

    // variant::inp(int;str;);
    if (currToken.getType() == TokenTypes::VARIANT_KW){
        auto variantTypeDef = parseVariantTypeDefinition();
        std::string id = variantTypeDef->getVariantName();
        if (!variantTypeDef)
            throw MyException("Invalid variant type definition", currToken.getPosition());
        if(!declaredIds.insert(id).second)
            throw MyException("redefiniton found in local variant type definition", currToken.getPosition());
        return std::move(variantTypeDef);
    }

    // typ zdefiniowany przez użytkownika: usr_defined::a = 5;
    if (currToken.getType() == TokenTypes::IDENTIFIER){
        std::string type = std::get<std::string>(currToken.getValue());
        Position typePos = currToken.getPosition();
        getNextToken();
        consumeToken(TokenTypes::DOUBLE_COLON, "Expected '::' after type name");
        if (currToken.getType() != TokenTypes::IDENTIFIER)
            throw MyException("Expected identifier after '::'", currToken.getPosition());
        std::string id = std::get<std::string>(currToken.getValue());
        Position idPos = currToken.getPosition();
        if(!declaredIds.insert(id).second)
            throw MyException("redefiniton found in local variable definition", currToken.getPosition());
        // mamy typ (variant albo struct) i id
        getNextToken();
        auto typeNode = std::make_unique<Nodes::Type>(type, typePos);
        auto typeDeclNode = std::make_unique<Nodes::TypeDecl>(std::move(typeNode), id, idPos);
        if (currToken.getType() == TokenTypes::PAREN_LEFT){ // to znaczy że zmienna struktury
            auto structVar = parseStructVarDeclaration(isMutable, std::move(typeDeclNode));
            if (!structVar)
                throw MyException("Invalid struct variable declaration", currToken.getPosition());
            return std::move(structVar);
        }
        if (currToken.getType() == TokenTypes::ASSIGN){ // to znaczy że zmienna typu variant
            auto variantVar = parseVariantVarDeclaration(std::move(typeDeclNode));
            if (!variantVar)
                throw MyException("Invalid variant variable declaration", currToken.getPosition());
            return std::move(variantVar);
        }
    }
    return nullptr;
}

// *********************************************************************************************************************
//                  Program Parsing
// *********************************************************************************************************************

std::unique_ptr<Nodes::Program> Parser::parseProgram() {
    if (currToken.getType() == TokenTypes::EOF_TOKEN)
        return nullptr;
    if (currToken.getType() == TokenTypes::SINGLE_COMMENT || currToken.getType() == TokenTypes::MULTILINE_COMMENT_START)
        getNextToken();
    while (parseFunction() || parseDeclaration())
        if (currToken.getType() == TokenTypes::EOF_TOKEN)
            break;

    return std::make_unique<Nodes::Program>(std::move(functions), std::move(variables), std::move(structTypes), std::move(variantTypes), currToken.getPosition());
}

bool Parser::parseFunction() {
    if (currToken.getType() != TokenTypes::FUN_KW)
        return false;
    auto funDecl = parseFunctionDeclaration();
    if (!funDecl)
        throw MyException("Invalid function declaration", currToken.getPosition());
    if(!checkIfFunctionExists(funDecl->getFunctionName()))
        throw MyException("Function with this id already exists", currToken.getPosition());
    functions.insert(std::make_pair(funDecl->getFunctionName(), std::move(funDecl)));
    return true;
}


bool Parser::parseDeclaration() {
    auto structTypeDef = parseStructTypeDefinition();
    if (structTypeDef){
        //checkIfStructTypeExists(structTypeDef->getStructName());
        structTypes.insert(std::make_pair(structTypeDef->getStructName(), std::move(structTypeDef)));
        return true;
    }
    auto variantTypeDef = parseVariantTypeDefinition();
    if (variantTypeDef){
        //checkIfVariantTypeExists(variantTypeDef->getVariantName());
        variantTypes.insert(std::make_pair(variantTypeDef->getVariantName(), std::move(variantTypeDef)));
        return true;
    }
    return(parseVarDeclaration());
}

bool Parser::parseVarDeclaration() {
    if (currToken.getType() == TokenTypes::FUN_KW)
        return false;
    bool isMutable = false;
    if (currToken.getType() == TokenTypes::MUT_KW){
        isMutable = true;
        getNextToken();
    }
    if (isIdType(currToken.getType())) {
        auto varDecl = parseSimpleVariableDeclaration(isMutable);
        if (!varDecl)
            throw MyException("Invalid variable declaration", currToken.getPosition());
        variables.insert(std::make_pair(varDecl->getIdentifier(), std::move(varDecl)));
        return true;
    }
    auto typeDecl = parseTypeDeclaration();
    if (!typeDecl) // przeparsowaliśmy funkcję albo podstawową zmienną albo typ więc tu musi być już jakaś zmienna typu zdefiniowanego przez użytkwonika
        throw MyException("Expected type declaration", currToken.getPosition());
    if (currToken.getType() == TokenTypes::PAREN_LEFT) { // to znaczy że zmienna stuktury
        auto structVarDecl = parseStructVarDeclaration(isMutable, std::move(typeDecl));
        if (!structVarDecl)
            throw MyException("Invalid struct variable declaration", currToken.getPosition());
        if(!checkIfStructTypeExists(structVarDecl->getTypeName()))
            throw MyException("Type with this id already exists", currToken.getPosition());
        variables.insert(std::make_pair(structVarDecl->getIdentifier(), std::move(structVarDecl)));
        return true;
    }
    // skoro nie ma nawiasu to znaczy że zmienna typu variant
    auto variantVarDecl = parseVariantVarDeclaration(std::move(typeDecl));
    if (!variantVarDecl)
        throw MyException("Invalid variant variable declaration", currToken.getPosition());
    if(!checkIfVariantTypeExists(variantVarDecl->getTypeName()))
        throw MyException("Type with this id already exists", currToken.getPosition());
    variables.insert(std::make_pair(variantVarDecl->getIdentifier(), std::move(variantVarDecl)));
    return true;
}
