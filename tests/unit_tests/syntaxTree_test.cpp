#include <gtest/gtest.h>
#include "syntaxTree.h"

TEST(NodeTest, RelOpTest) {
    Position pos({});
    Nodes::RelOp relOp(RelationalOperator::EQUAL, pos);
    EXPECT_EQ(relOp.getType(), RelationalOperator::EQUAL);
}

TEST(NodeTest, ArtmOpTest) {
    Position pos({});
    Nodes::ArtmOp termOp(ArtmOperator::PLUS, pos);
    EXPECT_EQ(termOp.getType(), ArtmOperator::PLUS);
}

TEST(NodeTest, FactorOpTest) {
    Position pos({});
    Nodes::FactorOp factorOp(FactorOperator::MULTIPLY, pos);
    EXPECT_EQ(factorOp.getType(), FactorOperator::MULTIPLY);
}

TEST(NodeTest, UnaryOpTest) {
    Position pos({});
    Nodes::UnaryOp unaryOp(UnaryOperator::NEGATE, pos);
    EXPECT_EQ(unaryOp.getType(), UnaryOperator::NEGATE);
}

TEST(NodeTest, CastOpTest) {
    Position pos({});
    Nodes::CastOp castOp(IdType::INT, pos);
    EXPECT_EQ(castOp.getType(), IdType ::INT);
}

TEST(NodeTest, RelOpNameTest) {
    Position pos({});
    Nodes::RelOp relOp(RelationalOperator ::EQUAL, pos);
    EXPECT_EQ(relOp.getNodeName(), "Relation Operator: EQUALS");
}

TEST(NodeTest, ArtmOpNameTest) {
    Position pos({});
    Nodes::ArtmOp termOp(ArtmOperator::PLUS, pos);
    EXPECT_EQ(termOp.getNodeName(), "Term Operator: PLUS");
}

TEST(NodeTest, FactorOpNameTest) {
    Position pos({});
    Nodes::FactorOp factorOp(FactorOperator ::MULTIPLY, pos);
    EXPECT_EQ(factorOp.getNodeName(), "Factor Operator: MULTIPLY");
}

TEST(NodeTest, UnaryOpNameTest) {
    Position pos({});
    Nodes::UnaryOp unaryOp(UnaryOperator ::NEGATE, pos);
    EXPECT_EQ(unaryOp.getNodeName(), "Unary Operator: NEGATE");
}

TEST(NodeTest, CastOpNameTest) {
    Position pos({});
    Nodes::CastOp castOp(IdType ::INT, pos);
    EXPECT_EQ(castOp.getNodeName(), "Cast Operator: INT");
}


TEST(UnaryExprTest, ConstructorWithUnaryOp) {
    auto unaryOp = std::make_unique<Nodes::UnaryOp>(UnaryOperator ::NEGATE, Position{1, 1});
    auto castExpr = std::make_unique<Nodes::CastingExpr>(nullptr, Position{1, 1});
    Nodes::UnaryExpr unaryExpr(std::move(unaryOp), std::move(castExpr), Position{1, 1});

    EXPECT_NE(unaryExpr.getUnaryOp(), nullptr);
    EXPECT_EQ(unaryExpr.getUnaryOp()->getType(), UnaryOperator ::NEGATE);
    EXPECT_NE(unaryExpr.getExpression(), nullptr);
    EXPECT_EQ(unaryExpr.getNodeName(), "UnaryExpr: NEGATE");
}

TEST(UnaryExprTest, ConstructorWithoutUnaryOp) {
    auto castExpr = std::make_unique<Nodes::CastingExpr>(nullptr, Position{1, 1});
    Nodes::UnaryExpr unaryExpr(std::move(castExpr), Position{1, 1});

    EXPECT_EQ(unaryExpr.getUnaryOp(), nullptr);
    EXPECT_NE(unaryExpr.getExpression(), nullptr);
    EXPECT_EQ(unaryExpr.getNodeName(), "UnaryExpr: ");
}

TEST(MulExprTest, ConstructorWithAllParameters) {
    auto unaryExpr = std::make_unique<Nodes::UnaryExpr>(nullptr, Position{1, 1});
    auto factorOp = std::make_unique<Nodes::FactorOp>(FactorOperator::MULTIPLY, Position{1, 1});
    auto rightExpr = std::make_unique<Nodes::MulExpr>(nullptr, Position{1, 1});

    Nodes::MulExpr mulExpr(std::move(unaryExpr), std::move(factorOp), std::move(rightExpr), Position{1, 1});

    EXPECT_NE(mulExpr.getLeftOperand(), nullptr);
    EXPECT_NE(mulExpr.getFactorOp(), nullptr);
    EXPECT_NE(mulExpr.getRightOperand(), nullptr);
    EXPECT_EQ(mulExpr.getFactorOp()->getType(), FactorOperator::MULTIPLY);
}

TEST(MulExprTest, ConstructorWithLeftOperandOnly) {
    auto unaryExpr = std::make_unique<Nodes::UnaryExpr>(nullptr, Position{1, 1});
    Nodes::MulExpr mulExpr(std::move(unaryExpr), Position{1, 1});

    EXPECT_NE(mulExpr.getLeftOperand(), nullptr);
    EXPECT_EQ(mulExpr.getFactorOp(), nullptr);
    EXPECT_EQ(mulExpr.getRightOperand(), nullptr);
}

TEST(ArtmExprTest, ConstructorWithAllParameters) {
    auto mulExpr = std::make_unique<Nodes::MulExpr>(nullptr, Position{1, 1});
    auto artmOp = std::make_unique<Nodes::ArtmOp>(ArtmOperator::PLUS, Position{1, 1});
    auto rightExpr = std::make_unique<Nodes::ArtmExpr>(nullptr, Position{1, 1});

    Nodes::ArtmExpr artmExpr(std::move(mulExpr), std::move(artmOp), std::move(rightExpr), Position{1, 1});

    EXPECT_NE(artmExpr.getLeftOperand(), nullptr);
    EXPECT_NE(artmExpr.getArtmOp(), nullptr);
    EXPECT_NE(artmExpr.getRightOperand(), nullptr);
    EXPECT_EQ(artmExpr.getArtmOp()->getType(), ArtmOperator::PLUS);
}

TEST(ArtmExprTest, ConstructorWithLeftOperandOnly) {
    auto mulExpr = std::make_unique<Nodes::MulExpr>(nullptr, Position{1, 1});
    Nodes::ArtmExpr artmExpr(std::move(mulExpr), Position{1, 1});

    EXPECT_NE(artmExpr.getLeftOperand(), nullptr);
    EXPECT_EQ(artmExpr.getArtmOp(), nullptr);
    EXPECT_EQ(artmExpr.getRightOperand(), nullptr);
}

TEST(RelExprTest, ConstructorWithAllParameters) {
    auto artmExpr = std::make_unique<Nodes::ArtmExpr>(nullptr, Position{1, 1});
    auto relOp = std::make_unique<Nodes::RelOp>(RelationalOperator::EQUAL, Position{1, 1});
    auto rightExpr = std::make_unique<Nodes::RelExpr>(nullptr, Position{1, 1});

    Nodes::RelExpr relExpr(std::move(artmExpr), std::move(relOp), std::move(rightExpr), Position{1, 1});

    EXPECT_NE(relExpr.getLeftOperand(), nullptr);
    EXPECT_NE(relExpr.getRelOp(), nullptr);
    EXPECT_NE(relExpr.getRightOperand(), nullptr);
    EXPECT_EQ(relExpr.getRelOp()->getType(), RelationalOperator::EQUAL);
}

TEST(RelExprTest, ConstructorWithLeftOperandOnly) {
    auto artmExpr = std::make_unique<Nodes::ArtmExpr>(nullptr, Position{1, 1});
    Nodes::RelExpr relExpr(std::move(artmExpr), Position{1, 1});

    EXPECT_NE(relExpr.getLeftOperand(), nullptr);
    EXPECT_EQ(relExpr.getRelOp(), nullptr);
    EXPECT_EQ(relExpr.getRightOperand(), nullptr);
}

TEST(AndExprTest, ConstructorWithBothOperands) {
    auto leftRelExpr = std::make_unique<Nodes::RelExpr>(nullptr, Position{1, 1});
    auto rightAndExpr = std::make_unique<Nodes::AndExpr>(nullptr, Position{1, 1});

    Nodes::AndExpr andExpr(std::move(leftRelExpr), std::move(rightAndExpr), Position{1, 1});

    EXPECT_NE(andExpr.getLeftOperand(), nullptr);
    EXPECT_NE(andExpr.getRightOperand(), nullptr);
    EXPECT_EQ(andExpr.getNodeName(), "AndExpr");
}

TEST(AndExprTest, ConstructorWithLeftOperandOnly) {
    auto leftRelExpr = std::make_unique<Nodes::RelExpr>(nullptr, Position{1, 1});
    Nodes::AndExpr andExpr(std::move(leftRelExpr), Position{1, 1});

    EXPECT_NE(andExpr.getLeftOperand(), nullptr);
    EXPECT_EQ(andExpr.getRightOperand(), nullptr);
    EXPECT_EQ(andExpr.getNodeName(), "AndExpr");
}

TEST(OrExprTest, ConstructorWithBothOperands) {
    auto leftAndExpr = std::make_unique<Nodes::AndExpr>(nullptr, Position{1, 1});
    auto rightOrExpr = std::make_unique<Nodes::OrExpr>(nullptr, Position{1, 1});

    Nodes::OrExpr orExpr(std::move(leftAndExpr), std::move(rightOrExpr), Position{1, 1});

    EXPECT_NE(orExpr.getLeftOperand(), nullptr);
    EXPECT_NE(orExpr.getRightOperand(), nullptr);
    EXPECT_EQ(orExpr.getNodeName(), "(or) Expression");
}

TEST(OrExprTest, ConstructorWithLeftOperandOnly) {
    auto leftAndExpr = std::make_unique<Nodes::AndExpr>(nullptr, Position{1, 1});
    Nodes::OrExpr orExpr(std::move(leftAndExpr), Position{1, 1});

    EXPECT_NE(orExpr.getLeftOperand(), nullptr);
    EXPECT_EQ(orExpr.getRightOperand(), nullptr);
    EXPECT_EQ(orExpr.getNodeName(), "(or) Expression");
}

TEST(ExpressionTest, Constructor) {
    auto orExpr = std::make_unique<Nodes::OrExpr>(nullptr, Position{1, 1});
    Nodes::Expression expression(std::move(orExpr), Position{1, 1});

    EXPECT_NE(expression.getExpression(), nullptr);
    EXPECT_EQ(expression.getNodeName(), "Expression");
}

TEST(BooleanLiteralTest, ConstructorAndGetter) {
    Nodes::BooleanLiteral booleanLiteral(true, Position{1, 1});
    EXPECT_EQ(booleanLiteral.getValue(), true);
    EXPECT_EQ(booleanLiteral.getNodeName(), "Boolean Literal: 1");
}

TEST(IntLiteralTest, ConstructorAndGetter) {
    Nodes::IntLiteral intLiteral(42, Position{1, 1});
    EXPECT_EQ(intLiteral.getValue(), 42);
    EXPECT_EQ(intLiteral.getNodeName(), "Number Literal: 42");
}

TEST(FloatLiteralTest, ConstructorAndGetter) {
    Nodes::FloatLiteral floatLiteral(3.14f, Position{1, 1});
    EXPECT_FLOAT_EQ(floatLiteral.getValue(), 3.14f);
    EXPECT_EQ(floatLiteral.getNodeName(), "Number Literal: 3.140000");
}

TEST(StringLiteralTest, ConstructorAndGetter) {
    Nodes::StringLiteral stringLiteral("Hello", Position{1, 1});
    EXPECT_EQ(stringLiteral.getValue(), "Hello");
    EXPECT_EQ(stringLiteral.getNodeName(), "String Literal: Hello");
}

TEST(IdentifierTest, ConstructorAndGetter) {
    Nodes::Identifier identifier("varName", Position{1, 1});
    EXPECT_EQ(identifier.getName(), "varName");
    EXPECT_EQ(identifier.getNodeName(), "Identifier: varName");
}

TEST(SyntaxTreeTest, FunctionCallStatementTest) {
    Position pos{1, 2};
    std::string functionName = "myFunction";

    std::unique_ptr<Nodes::Expression> arg1 = std::make_unique<Nodes::Expression>(nullptr, pos);
    std::unique_ptr<Nodes::Expression> arg2 = std::make_unique<Nodes::Expression>(nullptr, pos);

    std::vector<std::unique_ptr<Nodes::Expression>> arguments;
    arguments.push_back(std::move(arg1));
    arguments.push_back(std::move(arg2));
    Nodes::FunctionCallStatement functionCallStmt(functionName, std::move(arguments), pos);

    EXPECT_EQ(functionCallStmt.getNodeName(), "FunctionCallStatement: myFunction");
    EXPECT_EQ(functionCallStmt.getFunctionName(), functionName);

    const auto& retrievedArguments = functionCallStmt.getArguments();
    EXPECT_EQ(retrievedArguments.size(), 2);
}

TEST(SyntaxTreeTest, TypeTest) {
    Position pos{1, 2};
    Nodes::Type type(IdType::INT, pos);

    EXPECT_EQ(type.getNodeName(), "Type: INT");
    EXPECT_EQ(type.getTypeAsString(), "INT");
}

TEST(SyntaxTreeTest, TypeDeclTest) {
    Position pos{1, 2};
    std::unique_ptr<Nodes::Type> type = std::make_unique<Nodes::Type>(IdType::INT, pos);
    std::string identifier = "myType";
    Nodes::TypeDecl typeDecl(std::move(type), identifier, pos);

    EXPECT_EQ(typeDecl.getNodeName(), "TypeDecl: myType::INT");
    EXPECT_EQ(typeDecl.getType()->getTypeAsString(), "INT");
    EXPECT_EQ(typeDecl.getType()->getTypeAsString(), "INT");
}

TEST(SyntaxTreeTest, VariableDeclarationTest) {
    Position pos{1, 2};
    bool mut = true;
    std::unique_ptr<Nodes::TypeDecl> type = std::make_unique<Nodes::TypeDecl>(
            std::make_unique<Nodes::Type>(IdType::INT, pos), "myType", pos);
    std::unique_ptr<Nodes::Expression> value = std::make_unique<Nodes::Expression>(nullptr, pos);

    Nodes::VariableDeclaration varDecl(mut, std::move(type), std::move(value), pos);
    EXPECT_EQ(varDecl.getNodeName(), "VariableDeclaration: myType");
    EXPECT_EQ(varDecl.getIdentifier(), "myType");
    EXPECT_EQ(varDecl.isMutable(), true);
}

TEST(SyntaxTreeTest, StructTypeDefinitionTest) {
    Position pos{1, 2};
    std::string structName = "myStruct";
    std::vector<std::unique_ptr<Nodes::TypeDecl>> fields;

    Nodes::StructTypeDefinition structDef(structName, std::move(fields), pos);
    EXPECT_EQ(structDef.getNodeName(), "StructTypeDefinition: myStruct");
    EXPECT_EQ(structDef.getStructName(), structName);
}

TEST(SyntaxTreeTest, StructVarDeclarationTest) {
    Position pos{1, 2};
    bool mut = false;
    std::unique_ptr<Nodes::TypeDecl> type = std::make_unique<Nodes::TypeDecl>(
            std::make_unique<Nodes::Type>(IdType::INT, pos), "myType", pos);
    std::vector<std::unique_ptr<Nodes::Expression>> values;

    Nodes::StructVarDeclaration structVarDecl(mut, std::move(type), std::move(values), pos);

    EXPECT_EQ(structVarDecl.getNodeName(), "StructVarDeclaration: myType");
    EXPECT_EQ(structVarDecl.getIdentifier(), "myType");
    EXPECT_EQ(structVarDecl.isMutable(), false);
}

TEST(SyntaxTreeTest, VariantTypeDefinitionTest) {
    Position pos{1, 2};
    std::string variantName = "myVariant";
    std::vector<std::unique_ptr<Nodes::Type>> fields;

    Nodes::VariantTypeDefinition variantDef(variantName, std::move(fields), pos);
    EXPECT_EQ(variantDef.getNodeName(), "VariantTypeDefinition: myVariant");
    EXPECT_EQ(variantDef.getVariantName(), variantName);
}

TEST(SyntaxTreeTest, VariantVarDeclarationTest) {
    Position pos{1, 2};
    std::unique_ptr<Nodes::TypeDecl> typeDecl = std::make_unique<Nodes::TypeDecl>(
            std::make_unique<Nodes::Type>(IdType::INT, pos), "myType", pos);
    std::unique_ptr<Nodes::Expression> value = std::make_unique<Nodes::Expression>(nullptr, pos);

    Nodes::VariantVarDeclaration variantVarDecl(std::move(typeDecl), std::move(value), pos);
    EXPECT_EQ(variantVarDecl.getNodeName(), "VariantVarDeclaration: myType");
    EXPECT_EQ(variantVarDecl.getIdentifier(), "myType");
}

TEST(SyntaxTreeTest, AssignmentTest) {
    Position pos{1, 2};
    std::string identifier = "myVar";
    std::unique_ptr<Nodes::Expression> expression = std::make_unique<Nodes::Expression>(nullptr, pos);

    Nodes::Assignment assignment(identifier, std::move(expression), pos);

    EXPECT_EQ(assignment.getNodeName(), "Assignment: myVar");
    EXPECT_EQ(assignment.getIdentifier(), "myVar");
}

TEST(SyntaxTreeTest, StructFieldAssignmentTest) {
    Position pos{1, 2};
    std::string identifier = "myStruct";
    std::string fieldName = "myField";
    std::unique_ptr<Nodes::Expression> expression = std::make_unique<Nodes::Expression>(nullptr, pos);

    Nodes::StructFieldAssignment structFieldAssignment(identifier, fieldName, std::move(expression), pos);

    EXPECT_EQ(structFieldAssignment.getNodeName(), "StructFieldAssignment: myStruct.myField");
    EXPECT_EQ(structFieldAssignment.getIdentifier(), "myStruct");
    EXPECT_EQ(structFieldAssignment.getFieldName(), "myField");
}

TEST(SyntaxTreeTest, ReturnStatementTest) {
    Position pos{1, 2};
    std::unique_ptr<Nodes::Expression> expression = std::make_unique<Nodes::Expression>(nullptr, pos);

    Nodes::ReturnStatement returnStmt(std::move(expression), pos);
    EXPECT_EQ(returnStmt.getNodeName(), "ReturnStatement");
}
