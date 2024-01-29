#ifndef TKOM_PROJEKT_INTERPRETERVISITOR_H
#define TKOM_PROJEKT_INTERPRETERVISITOR_H

#include <unordered_map>
#include "syntaxTreeVisitor.h"
#include "symbolTableManager.h"

class InterpreterVisitor : public SyntaxTreeVisitor
{
private:
    SymbolTableManager symbolManager;
    std::optional<std::variant<IdType, std::string>> expectedType;
    std::variant<int, float, bool, std::string> currentValue;
    std::vector<std::variant<int, float, bool, std::string>> arguments;
    std::unordered_map<std::string, std::variant<int, float, bool, std::string>> variables;
    bool returned= false;
    const std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>>& structTypes;
    const std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>>& variantTypes;
public:
    std::unordered_map<std::string, std::variant<int, float, bool, std::string>> getVariables() { return variables; }
    InterpreterVisitor(const std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>>& structTypes,
                    const std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>>& variantTypes)
            : structTypes(structTypes), variantTypes(variantTypes) {}

    void visitBoolLiteral(Nodes::BooleanLiteral *) override;
    void visitIntLiteral(Nodes::IntLiteral *) override;
    void visitFloatLiteral(Nodes::FloatLiteral *) override;
    void visitStringLiteral(Nodes::StringLiteral *) override;
    void visitIdentifier(Nodes::Identifier *) override;
    void visitRelOp(Nodes::RelOp *) override;
    void visitArtmOp(Nodes::ArtmOp *) override;
    void visitFactorOp(Nodes::FactorOp *) override;
    void visitUnaryOp(Nodes::UnaryOp *) override;
    void visitCastOp(Nodes::CastOp *) override;
    void visitCastingExpr(Nodes::CastingExpr *) override;
    void visitUnaryExpr(Nodes::UnaryExpr *) override;
    void visitMulExpr(Nodes::MulExpr *) override;
    void visitArtmExpr(Nodes::ArtmExpr *) override;
    void visitRelExpr(Nodes::RelExpr *) override;
    void visitAndExpr(Nodes::AndExpr *) override;
    void visitOrExpr(Nodes::OrExpr *) override;
    void visitExpr(Nodes::Expression *) override;
    void visitFuncCall(Nodes::FunCall *) override;
    void visitVariableRef(Nodes::VarReference *) override;
    void visitDeclaration(Nodes::Declaration *) override;
    void visitType(Nodes::Type *) override;
    void visitTypeDecl(Nodes::TypeDecl *) override;
    void visitVariableDeclaration(Nodes::VariableDeclaration *) override;
    void visitStructTypeDefinition(Nodes::StructTypeDefinition *) override;
    void visitStructVarDeclaration(Nodes::StructVarDeclaration *) override;
    void visitVariantTypeDefinition(Nodes::VariantTypeDefinition *) override;
    void visitVariantVarDeclaration(Nodes::VariantVarDeclaration *) override;
    void visitAssignment(Nodes::Assignment *) override;
    void visitStructFieldAssignment(Nodes::StructFieldAssignment *) override;
    void visitReturnStatement(Nodes::ReturnStatement *) override;
    void visitBlock(Nodes::Block *) override;
    void visitIfStatement(Nodes::IfStatement *) override;
    void visitWhileStatement(Nodes::WhileStatement *) override;
    void visitFunctionCallStatement(Nodes::FunctionCallStatement *) override;
    void visitFunctionDeclaration(Nodes::FunctionDeclaration *) override;
    void visitProgram(Nodes::Program *) override;

    static bool valueToBool(std::variant<int, float, bool, std::string> variant);
};

#endif //TKOM_PROJEKT_INTERPRETERVISITOR_H
