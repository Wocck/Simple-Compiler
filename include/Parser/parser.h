#ifndef TKOM_PROJEKT_PARSER_H
#define TKOM_PROJEKT_PARSER_H

#include <map>
#include <set>
#include <algorithm>
#include "syntaxTree.h"
#include "lexer.h"
#include "token.h"

class Parser {
private:
    Lexer lexer;
    Token currToken;

    std::map<std::string, std::unique_ptr<Nodes::FunctionDeclaration>> functions;
    std::map<std::string, std::unique_ptr<Nodes::Declaration>> variables;
    std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>> structTypes;
    std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>> variantTypes;

    std::vector<std::string> structTypeNames;
    std::vector<std::string> variantTypeNames;

    // Helper functions
public:
    bool matchToken(TokenTypes) const;
    void getNextToken();
    void consumeToken(TokenTypes, const std::string&);
    bool isIdType(TokenTypes) const;
    bool isSimpleVarType(TokenTypes) const;
    IdType getIdTypeOfToken(TokenTypes) const;
    bool contains(const std::vector<std::string>&, const std::string&) const;
    bool checkIfStructTypeExists(const std::string&) const;
    bool checkIfVariantTypeExists(const std::string&)const;
    bool checkIfFunctionExists(const std::string&) const;
    bool checkIfVariableExists(const std::string&) const;

    std::unique_ptr<Nodes::FunctionCallStatement> parseFunctionCallStatement(std::string identifier);
    std::unique_ptr<Nodes::Assignment> parseAssignment(std::string identifier);
    std::unique_ptr<Nodes::StructFieldAssignment> parseStructFieldAssignment(std::string identifier);
// public:
    explicit Parser(std::istream& input_stream): lexer(input_stream), currToken(lexer.getNextToken()) {};
    explicit Parser(const std::string& file_name): lexer(file_name), currToken(lexer.getNextToken()) {};

    // Literals Parsing
    std::unique_ptr<Nodes::StringLiteral> parseStringLiteral();
    std::unique_ptr<Nodes::FloatLiteral> parseFloatLiteral();
    std::unique_ptr<Nodes::IntLiteral> parseIntLiteral();
    std::unique_ptr<Nodes::BooleanLiteral> parseBooleanLiteral();
    std::unique_ptr<Nodes::Identifier> parseIdentifier();

    // Operators Parsing
    std::unique_ptr<Nodes::RelOp> parseRelOp();
    std::unique_ptr<Nodes::ArtmOp> parseArtmOp();
    std::unique_ptr<Nodes::FactorOp> parseFactorOp();
    std::unique_ptr<Nodes::UnaryOp> parseUnaryOp();
    std::unique_ptr<Nodes::CastOp> parseCastOp();

    // Expressions Parsing
    std::unique_ptr<Nodes::Factor> parseFunctionCallOrVarRef();
    std::unique_ptr<Nodes::CastingExpr> parseCastingExpression();
    std::unique_ptr<Nodes::UnaryExpr> parseUnaryExpression();
    std::unique_ptr<Nodes::MulExpr> parseMulExpression();
    std::unique_ptr<Nodes::ArtmExpr> parseArtmExpression();
    std::unique_ptr<Nodes::RelExpr> parseRelExpression();
    std::unique_ptr<Nodes::AndExpr> parseAndExpression();
    std::unique_ptr<Nodes::OrExpr> parseOrExpression();
    std::unique_ptr<Nodes::Expression> parseExpression();
    std::unique_ptr<Nodes::Factor> parseFactor();

    // Statement Parsing
    std::unique_ptr<Nodes::FunctionDeclaration> parseFunctionDeclaration();
    std::unique_ptr<Nodes::WhileStatement> parseWhileStatement();
    std::unique_ptr<Nodes::IfStatement> parseIfStatement();
    std::unique_ptr<Nodes::Block> parseBlock();
    std::unique_ptr<Nodes::ReturnStatement> parseReturnStatement();
    std::unique_ptr<Nodes::Statement> parseAssignmentOrCallOrVar(std::set<std::string>&);
    std::unique_ptr<Nodes::Statement> parseStatement(std::set<std::string>&);
    std::vector<std::unique_ptr<Nodes::Expression>> parseArguments();

    // Declarations Parsing
    std::unique_ptr<Nodes::VariantTypeDefinition> parseVariantTypeDefinition();
    std::unique_ptr<Nodes::VariantVarDeclaration> parseVariantVarDeclaration(std::unique_ptr<Nodes::TypeDecl>);
    std::unique_ptr<Nodes::StructVarDeclaration> parseStructVarDeclaration(bool, std::unique_ptr<Nodes::TypeDecl>);
    std::unique_ptr<Nodes::StructTypeDefinition> parseStructTypeDefinition();
    std::unique_ptr<Nodes::VariableDeclaration> parseSimpleVariableDeclaration(bool);
    std::unique_ptr<Nodes::TypeDecl> parseTypeDeclaration();
    std::unique_ptr<Nodes::Type> parseType();
    std::unique_ptr<Nodes::Declaration> parseLocalVarDeclaration(std::set<std::string>&);

    // High-Level Parsing
    std::unique_ptr<Nodes::Program> parseProgram();
    bool parseFunction();
    bool parseDeclaration();
    bool parseVarDeclaration();
};

#endif //TKOM_PROJEKT_PARSER_H
