#ifndef TKOM_PROJEKT_SYNTAXTREE_H
#define TKOM_PROJEKT_SYNTAXTREE_H

#include <memory>
#include <map>
#include <utility>
#include <vector>
#include <variant>
#include <optional>
#include "charReader.h"

class SyntaxTreeVisitor;

typedef enum UnaryOperator {
    NEGATE,
    NEGATIVE
} UnaryOperator;

typedef enum LiteralType {
    INT_VALUE,
    FLOAT_VALUE,
    STR_VALUE,
    BOOLEAN_VALUE
} LiteralType;

typedef enum IdType {
    INT,
    FLOAT,
    STR,
    BOOLEAN,
    STRUCT,
    VARIANT
} IdType;

typedef enum ArtmOperator {
    PLUS,
    MINUS
} ArtmOperator;

typedef enum FactorOperator {
    MULTIPLY,
    DIVIDE
} FactorOperator;

typedef enum RelationalOperator {
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL
} RelationalOperator;

typedef enum NodeType {
    // Existing nodes
    RELOP,
    TERMOP,
    FACTOROP,
    SIMPEXPR,
    ID_NODE,
    FUNCCALLARGLIST,
    FUNCCALL,
    FACTOR,
    TERM,
    ARITHMETICEXPR,
    NUMBER_NODE,
    EXPR,
    STMT,
    STMTBLOCK,
    ASSIGN,
    IFSTMT,
    WHILESTMT,
    RETURNSTMT,
    FUNCCALLSTMT,
    TYPEDNODE,
    DECLARATION,
    FUNCDECLARGLIST,
    FUNCDECL,
    PROGRAM,

    // Added nodes
    BOOLEAN_EXPR,
    BOOL_OP, // for boolean operators like AND, OR, NEGATE
    LITERAL, // for handling literals like INT_VALUE, FLOAT_VALUE, STR_VALUE
    BOOLEAN_LITERAL, // TRUE_KW, FALSE_KW
    CONTROL_FLOW_STMT, // for IF_KW, ELSE_KW, WHILE_KW
    TYPE_DECLARATION, // for STRUCT_KW, VARIANT_KW, MUT_KW
    VARIABLE_DECLARATION, // specific for variable declarations
} NodeType;


class Node{
public:
    virtual ~Node() = default;
    virtual void accept(SyntaxTreeVisitor&) = 0;
    [[nodiscard]] Position getPos() const;
    [[nodiscard]] std::string getNodeName() const;
protected:
    Position pos{};
    std::string nodeName;
    friend std::ostream& operator<<(std::ostream& os, Node*);
};

namespace Nodes{
    extern const std::vector<std::string> relationalOpToStr;

    extern const std::vector<std::string> factorOpToStr;

    extern const std::vector<std::string> idTypesToStr;

    extern const std::vector<std::string> termOpToStr;

    extern const std::vector<std::string> literalTypeToStr;

    extern const std::vector<std::string> unaryTypeToStr;

    class RelOp: public Node {
    private:
        RelationalOperator relOp;
    public:
        RelOp(RelationalOperator relOp, Position pos) : relOp(relOp) {
            this->pos = pos;
            nodeName = "Relation Operator: " + relationalOpToStr[relOp];
        }
        [[nodiscard]] RelationalOperator getType() const { return relOp; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class ArtmOp: public Node {
    private:
        ArtmOperator termOp;
    public:
        ArtmOp(ArtmOperator termOp, Position pos) : termOp(termOp) {
            this->pos = pos;
            nodeName = "Term Operator: " + termOpToStr[termOp];
        }
        [[nodiscard]] ArtmOperator getType() const { return termOp; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class FactorOp: public Node {
    private:
        FactorOperator factorOp;
    public:
        FactorOp(FactorOperator factorOp, Position pos) : factorOp(factorOp) {
            this->pos = pos;
            nodeName = "Factor Operator: " + factorOpToStr[factorOp];
        }
        [[nodiscard]] FactorOperator getType() const { return factorOp; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class UnaryOp: public Node {
    private:
        UnaryOperator unaryOp;
    public:
        UnaryOp(UnaryOperator unaryOp, Position pos) : unaryOp(unaryOp) {
            this->pos = pos;
            nodeName = "Unary Operator: " + unaryTypeToStr[unaryOp];
        }
        [[nodiscard]] UnaryOperator getType() const { return unaryOp; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class CastOp: public Node {
    private:
        IdType castOp;
    public:
        CastOp(IdType castOp, Position pos) : castOp(castOp) {
            this->pos = pos;
            nodeName = "Cast Operator: " + idTypesToStr[castOp];
        }
        [[nodiscard]] IdType getType() const {  return castOp; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Factor : public Node {
    public:
        ~Factor() override = default;
        Factor() : Node() { nodeName = "Factor"; }
        void accept(SyntaxTreeVisitor &visitor) override =0;
    };

    // Expression hierarchy

    class CastingExpr: public Factor {
    private:
        std::unique_ptr<Factor> expression;
        std::unique_ptr<CastOp> castOp;
    public:
        CastingExpr(std::unique_ptr<Factor> expression, std::unique_ptr<CastOp> castOp, Position pos)
                : expression(std::move(expression)), castOp(std::move(castOp)) {
            this->pos = pos;
            nodeName = "CastingExpr: " + idTypesToStr[this->castOp->getType()];
        }

        CastingExpr(std::unique_ptr<Factor> expression, Position pos)
                : expression(std::move(expression)) {
            this->pos = pos;
            this->castOp = nullptr;
            nodeName = "CastingExpr: ";
        }

        [[nodiscard]] const Factor* getExpression() const {
            return expression.get();
        }

        [[nodiscard]] const CastOp* getCastOp() const {
            return castOp.get();
        }
        void acceptExpr(SyntaxTreeVisitor &visitor) const;
        void acceptCastOperator(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class UnaryExpr: public Factor {
    private:
        std::unique_ptr<UnaryOp> unaryOp;
        std::unique_ptr<CastingExpr> expression;
    public:
        UnaryExpr(std::unique_ptr<UnaryOp> unaryOp, std::unique_ptr<CastingExpr> expression, Position pos)
                : expression(std::move(expression)), unaryOp(std::move(unaryOp)) {
            this->pos = pos;
            nodeName = "UnaryExpr: " + unaryTypeToStr[this->unaryOp->getType()];
        }

        UnaryExpr(std::unique_ptr<CastingExpr> expression, Position pos)
                : expression(std::move(expression)){
            this->pos = pos;
            this->unaryOp = nullptr;
            nodeName = "UnaryExpr: ";
        }

        [[nodiscard]] const CastingExpr* getExpression() const {
            return expression.get();
        }

        [[nodiscard]] const UnaryOp* getUnaryOp() const {
            return unaryOp.get();
        }
        void acceptExpr(SyntaxTreeVisitor &visitor) const;
        void acceptUnaryOperator(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class MulExpr: public Factor {
    private:
        std::unique_ptr<UnaryExpr> left;
        std::unique_ptr<FactorOp> factorOp;
        std::unique_ptr<MulExpr> right;

    public:
        MulExpr(std::unique_ptr<UnaryExpr> left, std::unique_ptr<FactorOp> factorOp, std::unique_ptr<MulExpr> right, Position pos)
                : left(std::move(left)), right(std::move(right)), factorOp(std::move(factorOp)) {
            this->pos = pos;
            nodeName = "MulExpr";
        }

        MulExpr(std::unique_ptr<UnaryExpr> left, Position pos)
                : left(std::move(left)){
            this->pos = pos;
            this->right = nullptr;
            this->factorOp = nullptr;
            nodeName = "MulExpr";
        }

        [[nodiscard]] const UnaryExpr* getLeftOperand() const {
            return left.get();
        }

        [[nodiscard]] const MulExpr* getRightOperand() const {
            return right.get();
        }

        [[nodiscard]] const FactorOp* getFactorOp() const {
            return factorOp.get();
        }
        void acceptLeft(SyntaxTreeVisitor &visitor) const;
        void acceptFactorOperator(SyntaxTreeVisitor &visitor) const;
        void acceptRight(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class ArtmExpr: public Factor {
    private:
        std::unique_ptr<MulExpr> left;
        std::unique_ptr<ArtmOp> artmOp;
        std::unique_ptr<ArtmExpr> right;

    public:
        ArtmExpr(std::unique_ptr<MulExpr> left, std::unique_ptr<ArtmOp> artmOp, std::unique_ptr<ArtmExpr> right, Position pos)
                : left(std::move(left)), right(std::move(right)), artmOp(std::move(artmOp)) {
            this->pos = pos;
            nodeName = "ArtmExpr";
        }

        ArtmExpr(std::unique_ptr<MulExpr> left, Position pos)
                : left(std::move(left)) {
            this->right = nullptr;
            this->artmOp = nullptr;
            this->pos = pos;
            nodeName = "ArtmExpr";
        }

        [[nodiscard]] const MulExpr* getLeftOperand() const {
            return left.get();
        }

        [[nodiscard]] const ArtmExpr* getRightOperand() const {
            return right.get();
        }

        [[nodiscard]] const ArtmOp* getArtmOp() const {
            return artmOp.get();
        }

        void acceptLeft(SyntaxTreeVisitor &visitor) const;
        void acceptArtmOperator(SyntaxTreeVisitor &visitor) const;
        void acceptRight(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class RelExpr: public Factor {
    private:
        std::unique_ptr<ArtmExpr> left;
        std::unique_ptr<RelOp> relOp;
        std::unique_ptr<RelExpr> right;

    public:
        RelExpr(std::unique_ptr<ArtmExpr> left, std::unique_ptr<RelOp> relOp, std::unique_ptr<RelExpr> right, Position pos)
                : left(std::move(left)), right(std::move(right)), relOp(std::move(relOp)) {
            this->pos = pos;
            nodeName = "RelExpr";
        }

        RelExpr(std::unique_ptr<ArtmExpr> left, Position pos)
                : left(std::move(left)) {
            this->right = nullptr;
            this->relOp = nullptr;
            this->pos = pos;
            nodeName = "RelExpr";
        }

        [[nodiscard]] const ArtmExpr* getLeftOperand() const {
            return left.get();
        }

        [[nodiscard]] const RelExpr* getRightOperand() const {
            return right.get();
        }

        [[nodiscard]] const RelOp* getRelOp() const {
            return relOp.get();
        }

        void acceptLeft(SyntaxTreeVisitor &visitor) const;
        void acceptRelOperator(SyntaxTreeVisitor &visitor) const;
        void acceptRight(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class AndExpr: public Factor {
    private:
        std::unique_ptr<RelExpr> left;
        std::unique_ptr<AndExpr> right;
    public:
        AndExpr(std::unique_ptr<RelExpr> left, std::unique_ptr<AndExpr> right, Position pos)
                : left(std::move(left)), right(std::move(right)) {
            this->pos = pos;
            nodeName = "AndExpr";
        }

        AndExpr(std::unique_ptr<RelExpr> left, Position pos)
                : left(std::move(left)) {
            this->right = nullptr;
            this->pos = pos;
            nodeName = "AndExpr";
        }

        [[nodiscard]] const RelExpr* getLeftOperand() const {
            return left.get();
        }

        [[nodiscard]] const AndExpr* getRightOperand() const {
            return right.get();
        }
        void acceptLeft(SyntaxTreeVisitor &visitor) const;
        void acceptRight(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class OrExpr : public Factor {
    private:
        std::unique_ptr<AndExpr> left;
        std::unique_ptr<OrExpr> right;
    public:
        OrExpr(std::unique_ptr<AndExpr> left, std::unique_ptr<OrExpr> right, Position pos)
                : left(std::move(left)), right(std::move(right)) {
            this->pos = pos;
            nodeName = "(or) Expression";
        }

        OrExpr(std::unique_ptr<AndExpr> left, Position pos)
                : left(std::move(left)) {
            this->right = nullptr;
            this->pos = pos;
            nodeName = "(or) Expression";
        }

        [[nodiscard]] const AndExpr* getLeftOperand() const {
            return left.get();
        }

        [[nodiscard]] const OrExpr* getRightOperand() const {
            return right.get();
        }

        void acceptLeft(SyntaxTreeVisitor &visitor) const;
        void acceptRight(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Expression: public Factor {
    private:
        std::unique_ptr<OrExpr> expression;
    public:
        Expression(std::unique_ptr<OrExpr> expression, Position pos)
                : expression(std::move(expression)) {
            this->pos = pos;
            nodeName = "Expression";
        }
        [[nodiscard]] const OrExpr* getExpression() const {
            return expression.get();
        }
        void acceptExpr(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    // Factor
    class BooleanLiteral : public Factor {
    private:
        bool value;
    public:
        BooleanLiteral(bool value, Position pos) : value(value) {
            this->pos = pos;
            nodeName = "Boolean Literal: " + std::to_string(value);
        }
        [[nodiscard]] bool getValue() const { return value; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class IntLiteral : public Factor {
    private:
        int value;
    public:
        IntLiteral(int value, Position pos) : value(value) {
            this->pos = pos;
            nodeName = "Number Literal: " + std::to_string(value);
        }
        [[nodiscard]] int getValue() const { return value; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class FloatLiteral : public Factor {
    private:
        float value;
    public:
        FloatLiteral(float value, Position pos) : value(value) {
            this->pos = pos;
            nodeName = "Number Literal: " + std::to_string(value);
        }
        [[nodiscard]] float getValue() const{ return value; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class StringLiteral : public Factor {
    private:
        std::string value;
    public:
        StringLiteral(std::string value, Position pos) : value(std::move(value)) {
            this->pos = pos;
            nodeName = "String Literal: " + this->value;
        }
        [[nodiscard]] std::string getValue() const { return value; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Identifier : public Factor {
    private:
        std::string identifier;
    public:
        Identifier(std::string name, Position pos) : identifier(std::move(name)) {
            this->pos = pos;
            nodeName = "Identifier: " + identifier;
        }
        [[nodiscard]] std::string getName() const { return identifier; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class FunCall: public Factor {
    private:
        std::string funName;
        std::vector<std::unique_ptr<Expression>> arguments;
    public:
        FunCall(std::string functionName, std::vector<std::unique_ptr<Expression>> arguments, Position pos)
                : funName(std::move(functionName)), arguments(std::move(arguments)) {
            this->pos = pos;
            nodeName = "CallExpression: " + this->funName;
        }

        [[nodiscard]] std::string getIdentifier() const {
            return funName;
        }

        [[nodiscard]] std::optional<std::vector<Expression*>> getArguments() const;

        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class VarReference: public Factor {
    private:
        std::string identifier;
    public:
        VarReference(std::string identifier, Position pos)
                : identifier(std::move(identifier)) {
            this->pos = pos;
            nodeName = "VarReference: " + this->identifier;
        }
        [[nodiscard]] std::string getIdentifier() const { return identifier; }
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    // Statement hierarchy
    class Statement: public Node {
    public:
        ~Statement() override = default;
        explicit Statement() : Node() {
            nodeName = "Statement";
        };
        void accept(SyntaxTreeVisitor &visitor) override =0;
    };

    class Declaration: public Statement {
    public:
        ~Declaration() override = default;
        explicit Declaration() : Statement() {
            nodeName = "Declaration";
        };
        void accept(SyntaxTreeVisitor &visitor) override =0;
    };

    class Type : public Node {
    private:
        std::variant<IdType, std::string> type;
    public:
        Type(IdType type, Position pos)
                : type(type) {
            this->pos = pos;
            nodeName = "Type: " + idTypesToStr[type];
        }

        Type(std::string type, Position pos)
                : type(type) {
            this->pos = pos;
            nodeName = "Type: " + type;
        }

        [[nodiscard]] std::variant<IdType, std::string> getIdType() const { return type;}

        [[nodiscard]] std::string getTypeAsString() const {
            if (std::holds_alternative<IdType>(type))
                return idTypesToStr[std::get<IdType>(type)];
            else
                return std::get<std::string>(type);
        }

        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class TypeDecl : public Node {
    private:
        std::unique_ptr<Type> type;
        std::string identifier;
    public:
        TypeDecl(std::unique_ptr<Type> type, std::string identifier, Position pos)
                : type(std::move(type)), identifier(std::move(identifier)) {
            this->pos = pos;
            nodeName = "TypeDecl: " + this->identifier + "::" + this->type->getTypeAsString();
        }

        [[nodiscard]] Nodes::Type* getType() const {
            return type.get();
        }

        [[nodiscard]] std::string getIdentifier() const {
            return identifier;
        }

        void acceptType(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class VariableDeclaration: public Declaration {
    private:
        bool mut=false;
        std::unique_ptr<TypeDecl> type;
        std::unique_ptr<Expression> value;
    public:
        VariableDeclaration(bool mut, std::unique_ptr<TypeDecl> type, std::unique_ptr<Expression> value, Position pos)
                : mut(mut), type(std::move(type)), value(std::move(value)) {
            this->pos = pos;
            nodeName = "VariableDeclaration: " + this->type->getIdentifier();
        }

        [[nodiscard]] bool isMutable() const {
            return mut;
        }

        [[nodiscard]] TypeDecl* getTypeDecl() const {
            return type.get();
        }

        [[nodiscard]] Expression* getInitExpr() const {
            return value.get();
        }

        [[nodiscard]] std::string getIdentifier() const {
            return type->getIdentifier();
        }

        [[nodiscard]] std::string getTypeName() const {
            return type->getType()->getTypeAsString();
        }

        void acceptType(SyntaxTreeVisitor &visitor) const;
        void acceptInitExpr(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class StructTypeDefinition: public Declaration {
    private:
        std::string structName;
        std::vector<std::unique_ptr<TypeDecl>> fields;
    public:
        StructTypeDefinition(std::string name, std::vector<std::unique_ptr<TypeDecl>> fieldList, Position pos)
                : structName(std::move(name)), fields(std::move(fieldList)) {
            this->pos = pos;
            nodeName = "StructTypeDefinition: " + structName;
        }

        [[nodiscard]] std::string getStructName() const {
            return structName;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<TypeDecl>>& getFields() const {
            return fields;
        }

        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class StructVarDeclaration: public Declaration {
    private:
        bool mut=false;
        std::unique_ptr<TypeDecl> type;
        std::vector<std::unique_ptr<Expression>> values;
    public:
        StructVarDeclaration(bool mut, std::unique_ptr<TypeDecl> type, std::vector<std::unique_ptr<Expression>> values, Position pos)
                : mut(mut), type(std::move(type)), values(std::move(values)) {
            this->pos = pos;
            nodeName = "StructVarDeclaration: " + this->type->getIdentifier();
        }

        [[nodiscard]] bool isMutable() const {
            return mut;
        }

        [[nodiscard]] TypeDecl* getType() const {
            return type.get();
        }

        [[nodiscard]] std::vector<Expression*> getArgs() const;

        [[nodiscard]] std::string getIdentifier() const {
            return type->getIdentifier();
        }

        [[nodiscard]] std::string getTypeName() const {
            return std::get<std::string>(type->getType()->getIdType());
        }

        void acceptType(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class VariantTypeDefinition: public Declaration {
    private:
        std::string variantName;
        std::vector<std::unique_ptr<Type>> fields;
    public:
        VariantTypeDefinition(std::string name, std::vector<std::unique_ptr<Type>> fieldList, Position pos)
                : variantName(std::move(name)), fields(std::move(fieldList)) {
            this->pos = pos;
            nodeName = "VariantTypeDefinition: " + variantName;
        }

        [[nodiscard]] std::string getVariantName() const {
            return variantName;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Type>>& getFields() const {
            return fields;
        }

        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class VariantVarDeclaration: public Declaration {
    private:
        std::unique_ptr<TypeDecl> typeDecl;
        std::unique_ptr<Expression> value;
    public:
        VariantVarDeclaration(std::unique_ptr<TypeDecl> typeDecl, std::unique_ptr<Expression> value, Position pos)
                : typeDecl(std::move(typeDecl)), value(std::move(value)) {
            this->pos = pos;
            nodeName = "VariantVarDeclaration: " + this->typeDecl->getIdentifier();
        }

        [[nodiscard]] TypeDecl* getType() const {
            return typeDecl.get();
        }

        [[nodiscard]] Expression* getValue() const {
            return value.get();
        }

        [[nodiscard]] std::string getIdentifier() const {
            return typeDecl->getIdentifier();
        }

        [[nodiscard]] std::string getTypeName() const {
            return std::get<std::string>(typeDecl->getType()->getIdType());
        }

        void acceptType(SyntaxTreeVisitor &visitor) const;
        void acceptValue(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Assignment: public Statement {
    private:
        std::string identifier;
        std::unique_ptr<Expression> expression;
    public:
        Assignment(std::string identifier, std::unique_ptr<Expression> expression, Position pos)
                : identifier(std::move(identifier)), expression(std::move(expression)) {
            this->pos = pos;
            nodeName = "Assignment: " + this->identifier;
        }

        [[nodiscard]] std::string getIdentifier() const {
            return identifier;
        }

        [[nodiscard]] Expression* getExpression() const {
            return expression.get();
        }

        void acceptExpr(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class StructFieldAssignment : public Statement {
    private:
        std::string identifier;
        std::string fieldName;
        std::unique_ptr<Expression> expression;
    public:
        StructFieldAssignment(std::string identifier, std::string fieldName, std::unique_ptr<Expression> expression, Position pos)
                : identifier(std::move(identifier)), fieldName(std::move(fieldName)), expression(std::move(expression)) {
            this->pos = pos;
            nodeName = "StructFieldAssignment: " + this->identifier + "." + this->fieldName;
        }

        [[nodiscard]] std::string getIdentifier() const {
            return identifier;
        }

        [[nodiscard]] std::string getFieldName() const {
            return fieldName;
        }

        [[nodiscard]] Expression* getExpression() const {
            return expression.get();
        }

        void acceptExpr(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };


    class ReturnStatement: public Statement {
    private:
        std::unique_ptr<Expression> expression;
    public:
        ReturnStatement(std::unique_ptr<Expression> expression, Position pos)
                : expression(std::move(expression)) {
            this->pos = pos;
            nodeName = "ReturnStatement";
        }

        [[nodiscard]] Expression* getExpression() const {
            return expression.get();
        }

        void acceptReturnExpr(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Block : public Node {
    private:
        std::vector<std::unique_ptr<Statement>> statements;
    public:
        Block(std::vector<std::unique_ptr<Statement>> statements, Position pos)
                : statements(std::move(statements)) {
            this->pos = pos;
            nodeName = "Block";
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Statement>>& getStatements() const {
            return statements;
        }

        void acceptStatements(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class IfStatement: public Statement {
    private:
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> ifBlock;
        std::unique_ptr<Block> elseBlock;
    public:
        IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> ifBlock, std::unique_ptr<Block> elseBlock, Position pos)
                : condition(std::move(condition)), ifBlock(std::move(ifBlock)), elseBlock(std::move(elseBlock)) {
            this->pos = pos;
            nodeName = "IfStatement";
        }

        [[nodiscard]] Expression* getCondition() const {
            return condition.get();
        }

        [[nodiscard]] Block* getIfBlock() const {
            return ifBlock.get();
        }

        [[nodiscard]] Block* getElseBlock() const {
            return elseBlock.get();
        }

        void acceptCondition(SyntaxTreeVisitor &visitor) const;
        void acceptIfBlock(SyntaxTreeVisitor &visitor) const;
        void acceptElseBlock(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class WhileStatement: public Statement {
    private:
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> block;
    public:
        WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> block, Position pos)
                : condition(std::move(condition)), block(std::move(block)) {
            this->pos = pos;
            nodeName = "WhileStatement";
        }

        [[nodiscard]] Expression* getCondition() const {
            return condition.get();
        }

        [[nodiscard]] Block* getBlock() const {
            return block.get();
        }

        void acceptCondition(SyntaxTreeVisitor &visitor) const;
        void acceptWhileBlock(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class FunctionCallStatement: public Statement {
    private:
        std::string funName;
        std::vector<std::unique_ptr<Expression>> arguments;
    public:
        FunctionCallStatement(std::string functionName, std::vector<std::unique_ptr<Expression>> arguments, Position pos)
                : funName(std::move(functionName)), arguments(std::move(arguments)) {
            this->pos = pos;
            nodeName = "FunctionCallStatement: " + this->funName;
        }

        [[nodiscard]] std::string getFunctionName() const {
            return funName;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Expression>>& getArguments() const {
            return arguments;
        }

        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class FunctionDeclaration: public Node {
    private:
        std::unique_ptr<TypeDecl> returnType;
        std::vector<std::unique_ptr<TypeDecl>> parameters;
        std::unique_ptr<Block> block;
    public:
        FunctionDeclaration(std::unique_ptr<TypeDecl> returnType, std::vector<std::unique_ptr<TypeDecl>> parameters, std::unique_ptr<Block> block, Position pos)
                : returnType(std::move(returnType)), parameters(std::move(parameters)), block(std::move(block)) {
            this->pos = pos;
            nodeName = "FunctionDeclaration";
        }

        [[nodiscard]] TypeDecl* getReturnType() const {
            return returnType.get();
        }

        [[nodiscard]] std::optional<std::vector<TypeDecl*>> getParameters() const;

        [[nodiscard]] Block* getBlock() const {
            return block.get();
        }

        [[nodiscard]] std::string getFunctionName() const {
            return returnType->getIdentifier();
        }

        void acceptFunctionBody(SyntaxTreeVisitor &visitor) const;
        void acceptReturnType(SyntaxTreeVisitor &visitor) const;
        void accept(SyntaxTreeVisitor &visitor) override;
    };

    class Program: public Node{
    private:
        std::map<std::string, std::unique_ptr<Nodes::FunctionDeclaration>> functions;
        std::map<std::string, std::unique_ptr<Nodes::Declaration>> variables;
        std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>> structTypes;
        std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>> variantTypes;
    public:
        Program(std::map<std::string, std::unique_ptr<Nodes::FunctionDeclaration>> functions,
                std::map<std::string, std::unique_ptr<Nodes::Declaration>> variables,
                std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>> structTypes,
                std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>> variantTypes,
                Position pos)
                : functions(std::move(functions)), variables(std::move(variables)), structTypes(std::move(structTypes)), variantTypes(std::move(variantTypes)) {
            this->pos = pos;
            nodeName = "Program";
        }

        [[nodiscard]] const std::map<std::string, std::unique_ptr<Nodes::FunctionDeclaration>>& getFunctions() const {
            return functions;
        }

        [[nodiscard]] const std::map<std::string, std::unique_ptr<Nodes::Declaration>>& getVariables() const {
            return variables;
        }

        [[nodiscard]] const std::map<std::string, std::unique_ptr<Nodes::StructTypeDefinition>>& getStructTypes() const {
            return structTypes;
        }

        [[nodiscard]] const std::map<std::string, std::unique_ptr<Nodes::VariantTypeDefinition>>& getVariantTypes() const {
            return variantTypes;
        }

        void accept(SyntaxTreeVisitor &visitor) override;
    };
}

#endif //TKOM_PROJEKT_SYNTAXTREE_H
