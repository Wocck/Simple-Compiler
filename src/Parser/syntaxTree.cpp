#include "Parser/syntaxTree.h"
#include "visitorTemplate.h"

Position Node::getPos() const {
    return pos;
}

std::ostream &operator<<(std::ostream &os, Node *node) {
    os << "Node name: " << node->nodeName;
    return os;
}

std::string Node::getNodeName() const {
    return nodeName;
}

namespace Nodes {
    const std::vector<std::string> relationalOpToStr = {
            "EQUALS",
            "NOT EQUALS",
            "GREATER",
            "GREATER OR EQUAL",
            "LESS",
            "LESS OR EQUAL"
    };

    const std::vector<std::string> factorOpToStr = {
            "MULTIPLY",
            "DIVIDE"
    };

    const std::vector<std::string> idTypesToStr = {
            "INT",
            "FLOAT",
            "STR",
            "BOOLEAN",
            "STRUCT",
            "VARIANT"
    };

    const std::vector<std::string> termOpToStr ={
            "PLUS",
            "MINUS"
    };

    const std::vector<std::string> literalTypeToStr ={
            "INT_VALUE",
            "FLOAT_VALUE",
            "STR_VALUE",
            "BOOLEAN_VALUE"
    };

    const std::vector<std::string> unaryTypeToStr ={
            "NEGATE",
            "NEGATIVE"
    };

    void RelOp::accept(SyntaxTreeVisitor &visitor) {visitor.visitRelOp(this);}
    void ArtmOp::accept(SyntaxTreeVisitor &visitor) {visitor.visitArtmOp(this);}
    void FactorOp::accept(SyntaxTreeVisitor &visitor) {visitor.visitFactorOp(this);}
    void UnaryOp::accept(SyntaxTreeVisitor &visitor) {visitor.visitUnaryOp(this);}
    void CastOp::accept(SyntaxTreeVisitor &visitor) {visitor.visitCastOp(this);}

    void CastingExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitCastingExpr(this);}
    void CastingExpr::acceptExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }
    void CastingExpr::acceptCastOperator(SyntaxTreeVisitor &visitor) const {
        if (castOp)
            castOp->accept(visitor);
    }

    void UnaryExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitUnaryExpr(this);}
    void UnaryExpr::acceptExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }
    void UnaryExpr::acceptUnaryOperator(SyntaxTreeVisitor &visitor) const {
        if (unaryOp)
            unaryOp->accept(visitor);
    }

    void MulExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitMulExpr(this);}

    void MulExpr::acceptLeft(SyntaxTreeVisitor &visitor) const {
        if (left)
            left->accept(visitor);
    }
    void MulExpr::acceptFactorOperator(SyntaxTreeVisitor &visitor) const {
        if (factorOp)
            factorOp->accept(visitor);
    }
    void MulExpr::acceptRight(SyntaxTreeVisitor &visitor) const {
        if (right)
            right->accept(visitor);
    }

    void ArtmExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitArtmExpr(this);}

    void ArtmExpr::acceptLeft(SyntaxTreeVisitor &visitor) const {
        if (left)
            left->accept(visitor);
    }
    void ArtmExpr::acceptArtmOperator(SyntaxTreeVisitor &visitor) const {
        if (artmOp)
            artmOp->accept(visitor);
    }
    void ArtmExpr::acceptRight(SyntaxTreeVisitor &visitor) const {
        if (right)
            right->accept(visitor);
    }

    void RelExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitRelExpr(this);}
    void RelExpr::acceptLeft(SyntaxTreeVisitor &visitor) const {
        if (left)
            left->accept(visitor);
    }
    void RelExpr::acceptRelOperator(SyntaxTreeVisitor &visitor) const {
        if (relOp)
            relOp->accept(visitor);
    }
    void RelExpr::acceptRight(SyntaxTreeVisitor &visitor) const {
        if (right)
            right->accept(visitor);
    }

    void AndExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitAndExpr(this);}
    void AndExpr::acceptLeft(SyntaxTreeVisitor &visitor) const {
        if (left)
            left->accept(visitor);
    }
    void AndExpr::acceptRight(SyntaxTreeVisitor &visitor) const {
        if (right)
            right->accept(visitor);
    }

    void OrExpr::accept(SyntaxTreeVisitor &visitor) {visitor.visitOrExpr(this);}
    void OrExpr::acceptLeft(SyntaxTreeVisitor &visitor) const {
        if (left)
            left->accept(visitor);
    }
    void OrExpr::acceptRight(SyntaxTreeVisitor &visitor) const {
        if (right)
            right->accept(visitor);
    }
    void BooleanLiteral::accept(SyntaxTreeVisitor &visitor) {visitor.visitBoolLiteral(this);}
    void IntLiteral::accept(SyntaxTreeVisitor &visitor) {visitor.visitIntLiteral(this);}
    void FloatLiteral::accept(SyntaxTreeVisitor &visitor) {visitor.visitFloatLiteral(this);}
    void StringLiteral::accept(SyntaxTreeVisitor &visitor) {visitor.visitStringLiteral(this);}
    void Identifier::accept(SyntaxTreeVisitor &visitor) {visitor.visitIdentifier(this);}
    void Expression::accept(SyntaxTreeVisitor &visitor) {visitor.visitExpr(this);}

    void Expression::acceptExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }
    void FunCall::accept(SyntaxTreeVisitor &visitor) {visitor.visitFuncCall(this);}
    void VarReference::accept(SyntaxTreeVisitor &visitor) {visitor.visitVariableRef(this);}
    void Declaration::accept(SyntaxTreeVisitor &visitor) {visitor.visitDeclaration(this);}
    void Type::accept(SyntaxTreeVisitor &visitor) {visitor.visitType(this);}

    void TypeDecl::accept(SyntaxTreeVisitor &visitor) {visitor.visitTypeDecl(this);}
    void TypeDecl::acceptType(SyntaxTreeVisitor &visitor) const {
        if (type)
            type->accept(visitor);
    }

    void VariableDeclaration::accept(SyntaxTreeVisitor &visitor) {visitor.visitVariableDeclaration(this);}
    void VariableDeclaration::acceptType(SyntaxTreeVisitor &visitor) const {
        if (type)
            type->accept(visitor);
    }
    void VariableDeclaration::acceptInitExpr(SyntaxTreeVisitor &visitor) const {
        if (value)
            value->accept(visitor);
    }

    void StructTypeDefinition::accept(SyntaxTreeVisitor &visitor) {visitor.visitStructTypeDefinition(this);}
    void StructVarDeclaration::accept(SyntaxTreeVisitor &visitor) {visitor.visitStructVarDeclaration(this);}

    void VariantTypeDefinition::accept(SyntaxTreeVisitor &visitor) {visitor.visitVariantTypeDefinition(this);}
    void VariantVarDeclaration::accept(SyntaxTreeVisitor &visitor) {visitor.visitVariantVarDeclaration(this);}
    void VariantVarDeclaration::acceptType(SyntaxTreeVisitor &visitor) const {
        if (typeDecl)
            typeDecl->accept(visitor);
    }
    void VariantVarDeclaration::acceptValue(SyntaxTreeVisitor &visitor) const {
        if (value)
            value->accept(visitor);
    }

    void Assignment::accept(SyntaxTreeVisitor &visitor) {visitor.visitAssignment(this);}
    void Assignment::acceptExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }

    void StructFieldAssignment::accept(SyntaxTreeVisitor &visitor) {visitor.visitStructFieldAssignment(this);}
    void StructFieldAssignment::acceptExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }

    void ReturnStatement::accept(SyntaxTreeVisitor &visitor) {visitor.visitReturnStatement(this);}
    void ReturnStatement::acceptReturnExpr(SyntaxTreeVisitor &visitor) const {
        if (expression)
            expression->accept(visitor);
    }

    void Block::accept(SyntaxTreeVisitor &visitor) {visitor.visitBlock(this);}
    void Block::acceptStatements(SyntaxTreeVisitor &visitor) const {
        for (const auto & statement : statements) {
            statement->accept(visitor);
        }
    }

    void IfStatement::accept(SyntaxTreeVisitor &visitor) {visitor.visitIfStatement(this);}
    void IfStatement::acceptCondition(SyntaxTreeVisitor &visitor) const {
        if (condition)
            condition->accept(visitor);
    }
    void IfStatement::acceptIfBlock(SyntaxTreeVisitor &visitor) const {
        if (ifBlock)
            ifBlock->accept(visitor);
    }
    void IfStatement::acceptElseBlock(SyntaxTreeVisitor &visitor) const {
        if (elseBlock)
            elseBlock->accept(visitor);
    }

    void WhileStatement::accept(SyntaxTreeVisitor &visitor) {visitor.visitWhileStatement(this);}
    void WhileStatement::acceptCondition(SyntaxTreeVisitor &visitor) const {
        if (condition)
            condition->accept(visitor);
    }
    void WhileStatement::acceptWhileBlock(SyntaxTreeVisitor &visitor) const {
        if (block)
            block->accept(visitor);
    }

    void FunctionCallStatement::accept(SyntaxTreeVisitor &visitor) {visitor.visitFunctionCallStatement(this);}
    void FunctionDeclaration::accept(SyntaxTreeVisitor &visitor) {visitor.visitFunctionDeclaration(this);}
    std::optional<std::vector<TypeDecl *>> FunctionDeclaration::getParameters() const {
        if (parameters.empty())
            return std::nullopt;

        std::vector<TypeDecl *> args;
        args.reserve(parameters.size());
        for (const auto & parameter : parameters) {
            args.push_back(parameter.get());
        }
        return args;
    }
    void FunctionDeclaration::acceptFunctionBody(SyntaxTreeVisitor &visitor) const {
        if (block)
            block->accept(visitor);
    }
    void FunctionDeclaration::acceptReturnType(SyntaxTreeVisitor &visitor) const {
        if (returnType)
            returnType->accept(visitor);
    }

    std::optional<std::vector<Expression *>> FunCall::getArguments() const {
        if (arguments.empty())
            return std::nullopt;

        std::vector<Expression *> args;
        args.reserve(arguments.size());
        for (const auto & argument : arguments) {
            args.push_back(argument.get());
        }
        return args;
    }

    std::vector<Expression *> StructVarDeclaration::getArgs() const {
        std::vector<Expression *> args;
        args.reserve(values.size());
        for (const auto & argument : values) {
            args.push_back(argument.get());
        }
        return args;
    }

    void StructVarDeclaration::acceptType(SyntaxTreeVisitor &visitor) const {
        if (type)
            type->accept(visitor);
    }

    void Program::accept(SyntaxTreeVisitor &visitor) {visitor.visitProgram(this);}
}