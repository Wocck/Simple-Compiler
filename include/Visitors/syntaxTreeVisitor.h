#ifndef TKOM_PROJEKT_SYNTAXTREEVISITOR_H
#define TKOM_PROJEKT_SYNTAXTREEVISITOR_H

#include "syntaxTree.h"

class SyntaxTreeVisitor
{
public:
    virtual void visitBoolLiteral(Nodes::BooleanLiteral*) = 0;
    virtual void visitIntLiteral(Nodes::IntLiteral*) = 0;
    virtual void visitFloatLiteral(Nodes::FloatLiteral*) = 0;
    virtual void visitStringLiteral(Nodes::StringLiteral*) = 0;
    virtual void visitIdentifier(Nodes::Identifier*) = 0;

    virtual void visitRelOp(Nodes::RelOp*) = 0;
    virtual void visitArtmOp(Nodes::ArtmOp*) = 0;
    virtual void visitFactorOp(Nodes::FactorOp*) = 0;
    virtual void visitUnaryOp(Nodes::UnaryOp*) = 0;
    virtual void visitCastOp(Nodes::CastOp*) = 0;

    virtual void visitCastingExpr(Nodes::CastingExpr*) = 0;
    virtual void visitUnaryExpr(Nodes::UnaryExpr*) = 0;
    virtual void visitMulExpr(Nodes::MulExpr*) = 0;
    virtual void visitArtmExpr(Nodes::ArtmExpr*) = 0;
    virtual void visitRelExpr(Nodes::RelExpr*) = 0;
    virtual void visitAndExpr(Nodes::AndExpr*) = 0;
    virtual void visitOrExpr(Nodes::OrExpr*) = 0;
    virtual void visitExpr(Nodes::Expression*) = 0;

    virtual void visitFuncCall(Nodes::FunCall*) = 0;
    virtual void visitVariableRef(Nodes::VarReference*) = 0;
    virtual void visitDeclaration(Nodes::Declaration*) = 0;
    virtual void visitType(Nodes::Type*) = 0;
    virtual void visitTypeDecl(Nodes::TypeDecl*) = 0;
    virtual void visitVariableDeclaration(Nodes::VariableDeclaration*) = 0;
    virtual void visitStructTypeDefinition(Nodes::StructTypeDefinition*) = 0;
    virtual void visitStructVarDeclaration(Nodes::StructVarDeclaration*) = 0;
    virtual void visitVariantTypeDefinition(Nodes::VariantTypeDefinition*) = 0;
    virtual void visitVariantVarDeclaration(Nodes::VariantVarDeclaration*) = 0;

    virtual void visitAssignment(Nodes::Assignment*) = 0;
    virtual void visitStructFieldAssignment(Nodes::StructFieldAssignment*) = 0;
    virtual void visitReturnStatement(Nodes::ReturnStatement*) = 0;
    virtual void visitBlock(Nodes::Block*) = 0;
    virtual void visitIfStatement(Nodes::IfStatement*) = 0;
    virtual void visitWhileStatement(Nodes::WhileStatement*) = 0;
    virtual void visitFunctionCallStatement(Nodes::FunctionCallStatement*) = 0;
    virtual void visitFunctionDeclaration(Nodes::FunctionDeclaration*) = 0;
    virtual void visitProgram(Nodes::Program*) = 0;
};

#endif //TKOM_PROJEKT_SYNTAXTREEVISITOR_H
