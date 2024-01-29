#include <gtest/gtest.h>
#include <sstream>
#include "parser.h"

TEST(ParserTest, ParsesStringLiteral) {
    std::istringstream strStream("\"Hello, world!\"");
    Parser parser(strStream);
    auto stringLiteral = parser.parseStringLiteral();
    ASSERT_NE(stringLiteral, nullptr);
    EXPECT_EQ(stringLiteral->getValue(), "Hello, world!");
}

TEST(ParserTest, ParsesFloatLiteral) {
    std::istringstream strStream("3.14159");
    Parser parser(strStream);
    auto floatLiteral = parser.parseFloatLiteral();
    ASSERT_NE(floatLiteral, nullptr);
    EXPECT_FLOAT_EQ(floatLiteral->getValue(), 3.14159f);
}

TEST(ParserTest, ParsesIntLiteral) {
    std::istringstream strStream("  42  ");
    Parser parser(strStream);
    auto intLiteral = parser.parseIntLiteral();
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getValue(), 42);
}

TEST(ParserTest, ParsesBooleanLiteral) {
    std::istringstream strStream("  true  ");
    Parser parser(strStream);
    auto booleanLiteral = parser.parseBooleanLiteral();
    ASSERT_NE(booleanLiteral, nullptr);
    EXPECT_TRUE(booleanLiteral->getValue());
}

TEST(ParserTest, ParsesIdentifier) {
    std::istringstream strStream("  myVar  ");
    Parser parser(strStream);
    auto identifier = parser.parseIdentifier();
    ASSERT_NE(identifier, nullptr);
    EXPECT_EQ(identifier->getName(), "myVar");
}

TEST(ParserTest, ParsesRelOpEqual) {
    std::istringstream strStream("==");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::EQUAL);
}

TEST(ParserTest, ParsesRelOpNotEqual) {
    std::istringstream strStream("!=");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::NOT_EQUAL);
}

TEST(ParserTest, ParsesRelOpLess) {
    std::istringstream strStream("<");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::LESS);
}

TEST(ParserTest, ParsesRelOpLessEqual) {
    std::istringstream strStream("<=");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::LESS_EQUAL);
}

TEST(ParserTest, ParsesRelOpGreater) {
    std::istringstream strStream(">");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::GREATER);
}

TEST(ParserTest, ParsesRelOpGreaterEqual) {
    std::istringstream strStream(">=");
    Parser parser(strStream);
    auto relOp = parser.parseRelOp();
    ASSERT_NE(relOp, nullptr);
    EXPECT_EQ(relOp->getType(), RelationalOperator::GREATER_EQUAL);
}

TEST(ParserTest, ParsesArtmOp) {
    std::istringstream strStream("+");
    Parser parser(strStream);
    auto artmOp = parser.parseArtmOp();
    ASSERT_NE(artmOp, nullptr);
    EXPECT_EQ(artmOp->getType(), ArtmOperator::PLUS);
}

TEST(ParserTest, ParsesFactorOp) {
    std::istringstream strStream("*");
    Parser parser(strStream);
    auto factorOp = parser.parseFactorOp();
    ASSERT_NE(factorOp, nullptr);
    EXPECT_EQ(factorOp->getType(), FactorOperator::MULTIPLY);
}

TEST(ParserTest, ParsesUnaryOp) {
    std::istringstream strStream("-");
    Parser parser(strStream);
    auto unaryOp = parser.parseUnaryOp();
    ASSERT_NE(unaryOp, nullptr);
    EXPECT_EQ(unaryOp->getType(), UnaryOperator::NEGATIVE);
}

TEST(ParserTest, ParsesFunctionCallOrVarRef) {
    std::istringstream strStream("myFunction()");
    Parser parser(strStream);
    auto funCallOrVarRef = parser.parseFunctionCallOrVarRef();
    ASSERT_NE(funCallOrVarRef, nullptr);
    auto funCall = dynamic_cast<Nodes::FunCall*>(funCallOrVarRef.get());
    ASSERT_NE(funCall, nullptr);
    EXPECT_EQ(funCall->getIdentifier(), "myFunction");
}

TEST(ParserTest, ParsesCastingExpression) {
    std::istringstream strStream("123 as[int]");
    Parser parser(strStream);
    auto castingExpr = parser.parseCastingExpression();
    ASSERT_NE(castingExpr, nullptr);


    const auto* intLiteral = dynamic_cast<const Nodes::IntLiteral*>(castingExpr->getExpression());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getValue(), 123);

    EXPECT_EQ(castingExpr->getCastOp()->getType(), IdType::INT);
}

TEST(ParserTest, ParsesUnaryExpression) {
    std::istringstream strStream("-42");
    Parser parser(strStream);
    auto unaryExpr = parser.parseUnaryExpression();
    ASSERT_NE(unaryExpr, nullptr);

    const auto* castingExpr = dynamic_cast<const Nodes::CastingExpr*>(unaryExpr->getExpression());
    ASSERT_NE(castingExpr, nullptr);


    const auto* intLiteral = dynamic_cast<const Nodes::IntLiteral*>(castingExpr->getExpression());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getValue(), -42);
}

TEST(ParserTest, ParsesMulExpression) {
    std::istringstream strStream("3 * 4");
    Parser parser(strStream);
    auto mulExpr = parser.parseMulExpression();
    ASSERT_NE(mulExpr, nullptr);


    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(mulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);

    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 3);


    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(mulExpr->getRightOperand());
    ASSERT_NE(rightMulExpr, nullptr);

    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);

    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 4);
}

TEST(ParserTest, ParsesArtmExpression) {
    std::istringstream strStream("5 + 10");
    Parser parser(strStream);
    auto artmExpr = parser.parseArtmExpression();
    ASSERT_NE(artmExpr, nullptr);

    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(artmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftMulExpr->getLeftOperand()->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 5);

    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(artmExpr->getRightOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightMulExpr->getLeftOperand()->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 10);
}

TEST(ParserTest, ParsesRelExpression) {
    std::istringstream strStream("10 > 5");
    Parser parser(strStream);
    auto relExpr = parser.parseRelExpression();
    ASSERT_NE(relExpr, nullptr);

    const auto* leftArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(relExpr->getLeftOperand());
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftArtmExpr->getLeftOperand()->getLeftOperand()->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 10);

    const auto* rightRelExpr = dynamic_cast<const Nodes::RelExpr*>(relExpr->getRightOperand());
    ASSERT_NE(rightRelExpr, nullptr);
    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(rightRelExpr->getLeftOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightArtmExpr->getLeftOperand()->getLeftOperand()->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 5);
}

TEST(ParserTest, ParsesAndExpression) {
    std::istringstream strStream("true and false");
    Parser parser(strStream);
    auto andExpr = parser.parseAndExpression();
    ASSERT_NE(andExpr, nullptr);

    const auto* leftRelExpr = dynamic_cast<const Nodes::RelExpr*>(andExpr->getLeftOperand());
    ASSERT_NE(leftRelExpr, nullptr);
    const auto* leftArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(leftRelExpr->getLeftOperand());
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(leftArtmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(leftMulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);
    const auto* boolLiteralLeft = dynamic_cast<const Nodes::BooleanLiteral*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(boolLiteralLeft, nullptr);
    EXPECT_TRUE(boolLiteralLeft->getValue());

    const auto* rightAndExpr = dynamic_cast<const Nodes::AndExpr*>(andExpr->getRightOperand());
    ASSERT_NE(rightAndExpr, nullptr);
    const auto* rightRelExpr = dynamic_cast<const Nodes::RelExpr*>(rightAndExpr->getLeftOperand());
    ASSERT_NE(rightRelExpr, nullptr);
    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(rightRelExpr->getLeftOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);
    const auto* boolLiteralRight = dynamic_cast<const Nodes::BooleanLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(boolLiteralRight, nullptr);
    EXPECT_FALSE(boolLiteralRight->getValue());
}

TEST(ParserTest, ParsesOrExpression) {
    std::istringstream strStream("false or true");
    Parser parser(strStream);
    auto orExpr = parser.parseOrExpression();
    ASSERT_NE(orExpr, nullptr);


    const auto* leftAndExpr = dynamic_cast<const Nodes::AndExpr*>(orExpr->getLeftOperand());
    ASSERT_NE(leftAndExpr, nullptr);
    const auto* leftRelExpr = dynamic_cast<const Nodes::RelExpr*>(leftAndExpr->getLeftOperand());
    ASSERT_NE(leftRelExpr, nullptr);
    const auto* leftArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(leftRelExpr->getLeftOperand());
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(leftArtmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(leftMulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);
    const auto* boolLiteralLeft = dynamic_cast<const Nodes::BooleanLiteral*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(boolLiteralLeft, nullptr);
    EXPECT_FALSE(boolLiteralLeft->getValue());


    const auto* rightOrExpr = dynamic_cast<const Nodes::OrExpr*>(orExpr->getRightOperand());
    ASSERT_NE(rightOrExpr, nullptr);
    const auto* rightAndExpr = dynamic_cast<const Nodes::AndExpr*>(rightOrExpr->getLeftOperand());
    ASSERT_NE(rightAndExpr, nullptr);
    const auto* rightRelExpr = dynamic_cast<const Nodes::RelExpr*>(rightAndExpr->getLeftOperand());
    ASSERT_NE(rightRelExpr, nullptr);
    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(rightRelExpr->getLeftOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);
    const auto* boolLiteralRight = dynamic_cast<const Nodes::BooleanLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(boolLiteralRight, nullptr);
    EXPECT_TRUE(boolLiteralRight->getValue());
}


TEST(ParserTest, ParsesIdentifierv) {
    std::istringstream strStream("variableName");
    Parser parser(strStream);
    auto identifier = parser.parseIdentifier();
    ASSERT_NE(identifier, nullptr);
    EXPECT_EQ(identifier->getName(), "variableName");
}

TEST(ParserTest, ParsesStringLiteralv) {
    std::istringstream strStream("\"Hello, world!\"");
    Parser parser(strStream);
    auto stringLiteral = parser.parseStringLiteral();
    ASSERT_NE(stringLiteral, nullptr);
    EXPECT_EQ(stringLiteral->getValue(), "Hello, world!");
}

TEST(ParserTest, ParsesRelLessExpression) {
    std::istringstream strStream("5 <= 10");
    Parser parser(strStream);
    auto relExpr = parser.parseRelExpression();
    ASSERT_NE(relExpr, nullptr);

    ASSERT_EQ(relExpr->getRelOp()->getType(), RelationalOperator::LESS_EQUAL);


    const auto* leftArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(relExpr->getLeftOperand());
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(leftArtmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(leftMulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);
    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 5);

    const auto* rightRelExpr = dynamic_cast<const Nodes::RelExpr*>(relExpr->getRightOperand());
    ASSERT_NE(rightRelExpr, nullptr);
    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(rightRelExpr->getLeftOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);
    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 10);
}

TEST(ParserTest, ParsesArtmPlusExpression) {
    std::istringstream strStream("3 + 2");
    Parser parser(strStream);
    auto artmExpr = parser.parseArtmExpression();
    ASSERT_NE(artmExpr, nullptr);

    ASSERT_EQ(artmExpr->getArtmOp()->getType(), ArtmOperator::PLUS);

    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(artmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(leftMulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);
    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 3);

    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(artmExpr->getRightOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);
    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 2);
}

TEST(ParserTest, ParsesRelGreaterEqualExpression) {
    std::istringstream strStream("ala >= 10");
    Parser parser(strStream);
    auto relExpr = parser.parseRelExpression();
    ASSERT_NE(relExpr, nullptr);

    ASSERT_EQ(relExpr->getRelOp()->getType(), RelationalOperator::GREATER_EQUAL);

    const auto* leftArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(relExpr->getLeftOperand());
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* leftMulExpr = dynamic_cast<const Nodes::MulExpr*>(leftArtmExpr->getLeftOperand());
    ASSERT_NE(leftMulExpr, nullptr);
    const auto* leftUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(leftMulExpr->getLeftOperand());
    ASSERT_NE(leftUnaryExpr, nullptr);
    const auto* idLiteralLeft = dynamic_cast<const Nodes::Identifier*>(leftUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(idLiteralLeft, nullptr);
    EXPECT_EQ(idLiteralLeft->getName(), "ala");

    const auto* rightRelExpr = dynamic_cast<const Nodes::RelExpr*>(relExpr->getRightOperand());
    ASSERT_NE(rightRelExpr, nullptr);
    const auto* rightArtmExpr = dynamic_cast<const Nodes::ArtmExpr*>(rightRelExpr->getLeftOperand());
    ASSERT_NE(rightArtmExpr, nullptr);
    const auto* rightMulExpr = dynamic_cast<const Nodes::MulExpr*>(rightArtmExpr->getLeftOperand());
    ASSERT_NE(rightMulExpr, nullptr);
    const auto* rightUnaryExpr = dynamic_cast<const Nodes::UnaryExpr*>(rightMulExpr->getLeftOperand());
    ASSERT_NE(rightUnaryExpr, nullptr);
    const auto* intLiteralRight = dynamic_cast<const Nodes::IntLiteral*>(rightUnaryExpr->getExpression()->getExpression());
    ASSERT_NE(intLiteralRight, nullptr);
    EXPECT_EQ(intLiteralRight->getValue(), 10);
}

TEST(ParserTest, ParsesRelEqualExpression) {
    std::istringstream strStream("7 == 8");
    Parser parser(strStream);
    auto relExpr = parser.parseRelExpression();
    ASSERT_NE(relExpr, nullptr);

    const auto* leftArtmExpr = relExpr->getLeftOperand();
    ASSERT_NE(leftArtmExpr, nullptr);
    const auto* intLiteralLeft = dynamic_cast<const Nodes::IntLiteral*>(leftArtmExpr->getLeftOperand()->getLeftOperand()->getExpression()->getExpression());
    ASSERT_NE(intLiteralLeft, nullptr);
    EXPECT_EQ(intLiteralLeft->getValue(), 7);
}

TEST(ParserTest, ParsesFunctionCall) {
    std::istringstream strStream("myFunction(1, 2, 3)");
    Parser parser(strStream);
    auto functionCallOrVarRef = parser.parseFunctionCallOrVarRef();
    ASSERT_NE(functionCallOrVarRef, nullptr);

    auto functionCall = dynamic_cast<Nodes::FunCall*>(functionCallOrVarRef.get());
    ASSERT_NE(functionCall, nullptr);
    EXPECT_EQ(functionCall->getIdentifier(), "myFunction");

    const auto& arguments = functionCall->getArguments();
}

TEST(ParserTest, ParsesFunctionCallWithNoArguments) {
    std::istringstream strStream("myFunction()");
    Parser parser(strStream);
    auto functionCallOrVarRef = parser.parseFunctionCallOrVarRef();
    ASSERT_NE(functionCallOrVarRef, nullptr);

    auto functionCall = dynamic_cast<Nodes::FunCall*>(functionCallOrVarRef.get());
    ASSERT_NE(functionCall, nullptr);
    EXPECT_EQ(functionCall->getIdentifier(), "myFunction");

    const auto& arguments = functionCall->getArguments();
}

TEST(ParserTest, ParsesVariantTypeDefinition) {
    std::istringstream strStream("variant::Choice(int; float; str;);");
    Parser parser(strStream);
    auto variantType = parser.parseVariantTypeDefinition();
    ASSERT_NE(variantType, nullptr);
    EXPECT_EQ(variantType->getVariantName(), "Choice");
    const auto& types = variantType->getFields();
    ASSERT_EQ(types.size(), 3);
    EXPECT_EQ(types[0]->getTypeAsString(), "INT");
    EXPECT_EQ(types[1]->getTypeAsString(), "FLOAT");
    EXPECT_EQ(types[2]->getTypeAsString(), "STR");
}

TEST(ParserTest, ParsesVariantVarDeclaration) {
    std::istringstream strStream("Choice = 5;");
    Parser parser(strStream);
    auto typeDecl = std::make_unique<Nodes::TypeDecl>(std::make_unique<Nodes::Type>("Choice", Position()), "Choice", Position());
    parser.getNextToken();
    auto variantVar = parser.parseVariantVarDeclaration(std::move(typeDecl));
    ASSERT_NE(variantVar, nullptr);
    EXPECT_EQ(variantVar->getTypeName(), "Choice");
    auto expr = variantVar->getValue();
    ASSERT_NE(expr, nullptr);
}

TEST(ParserTest, ParsesStructVarDeclaration) {
    std::istringstream strStream("Position(3, 4);");
    Parser parser(strStream);
    auto typeDecl = std::make_unique<Nodes::TypeDecl>(std::make_unique<Nodes::Type>("Position", Position()), "pos", Position());
    parser.getNextToken();
    auto structVar = parser.parseStructVarDeclaration(false, std::move(typeDecl));
    ASSERT_NE(structVar, nullptr);
    EXPECT_EQ(structVar->getTypeName(), "Position");
    const auto& args = structVar->getArgs();
    ASSERT_EQ(args.size(), 2);
}

TEST(ParserTest, ParsesStructTypeDefinition) {
    std::istringstream strStream("struct::Position(int::x; float::y;);");
    Parser parser(strStream);
    auto structType = parser.parseStructTypeDefinition();
    ASSERT_NE(structType, nullptr);
    EXPECT_EQ(structType->getStructName(), "Position");
    const auto& members = structType->getFields();
    ASSERT_EQ(members.size(), 2);
    EXPECT_EQ(members[0]->getIdentifier(), "x");
    EXPECT_EQ(members[1]->getIdentifier(), "y");
}

TEST(ParserTest, ParsesSimpleVariableDeclaration) {
    std::istringstream strStream("int::number = 42;");
    Parser parser(strStream);
    auto varDecl = parser.parseSimpleVariableDeclaration(false);
    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->getIdentifier(), "number");
    auto initExpr = varDecl->getInitExpr();
    ASSERT_NE(initExpr, nullptr);
}

TEST(ParserTest, ParsesType) {
    std::istringstream strStream("int");
    Parser parser(strStream);
    auto type = parser.parseType();
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->getTypeAsString(), "INT");
}

TEST(ParserTest, ParsesTypeMy) {
    std::istringstream strStream("MyType");
    Parser parser = Parser(strStream);
    auto type = parser.parseType();
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->getTypeAsString(), "MyType");
}

TEST(ParserTest, ParsesLocalVarDeclaration) {
    std::istringstream strStream("mut int::number = 42;");
    Parser parser(strStream);
    std::set<std::string> declaredIds;
    auto localVarDecl = parser.parseLocalVarDeclaration(declaredIds);
    ASSERT_NE(localVarDecl, nullptr);
    auto varDecl = dynamic_cast<Nodes::VariableDeclaration*>(localVarDecl.get());
    ASSERT_NE(varDecl, nullptr);
    EXPECT_TRUE(varDecl->isMutable());
    EXPECT_EQ(varDecl->getIdentifier(), "number");
}

TEST(ParserTest, ParsesProgram) {
    std::istringstream strStream("mut int::myVar = 10; fun int::myFunction()[ return 5; ]");
    Parser parser(strStream);
    auto program = parser.parseProgram();

    ASSERT_NE(program, nullptr);

    const auto& functions = program->getFunctions();
    ASSERT_EQ(functions.size(), 1);

    const auto& function = functions.at("myFunction");
    ASSERT_NE(function, nullptr);
    ASSERT_EQ(function->getFunctionName(), "myFunction");
    ASSERT_EQ(std::get<IdType>(function->getReturnType()->getType()->getIdType()), IdType::INT);
    ASSERT_TRUE(function->getParameters()->empty());

    const auto& variables = program->getVariables();
    ASSERT_EQ(variables.size(), 1);

    auto var = dynamic_cast<Nodes::VariableDeclaration*>(variables.at("myVar").get());
    ASSERT_NE(var, nullptr);
    ASSERT_EQ(var->getIdentifier(), "myVar");
    ASSERT_EQ(std::get<IdType>(var->getTypeDecl()->getType()->getIdType()), IdType::INT);
    ASSERT_EQ(var->isMutable(), true);
}

TEST(ParserTest, ParsesIfStatement) {
    std::istringstream strStream("if (true) [ return 1; ] else [ return 2; ]");
    Parser parser(strStream);

    auto ifStatement = parser.parseIfStatement();
    ASSERT_NE(ifStatement, nullptr);
    const auto& condition = ifStatement->getCondition();
    ASSERT_NE(condition, nullptr);
    const auto& ifBlock = ifStatement->getIfBlock();
    ASSERT_NE(ifBlock, nullptr);
    const auto& elseBlock = ifStatement->getElseBlock();
    ASSERT_NE(elseBlock, nullptr);
}

TEST(ParserTest, ParsesWhileStatement) {
    std::istringstream strStream("while x < 5 [ x = x + 1; ]");
    Parser parser(strStream);
    auto whileStatement = parser.parseWhileStatement();
    ASSERT_NE(whileStatement, nullptr);
    const auto& condition = whileStatement->getCondition();
    auto relExpr = dynamic_cast<const Nodes::RelExpr*>(condition->getExpression()->getLeftOperand()->getLeftOperand());
    ASSERT_EQ(relExpr->getRelOp()->getType(), RelationalOperator::LESS);
    ASSERT_NE(condition, nullptr);
    const auto& bodyBlock = whileStatement->getBlock();
    ASSERT_NE(bodyBlock, nullptr);
    const auto& statement = bodyBlock->getStatements()[0];
    ASSERT_NE(statement, nullptr);
    ASSERT_EQ(statement->getNodeName(), "Assignment: x");
}

TEST(ParserTest, ParsesProgramB) {
    std::istringstream strStream("mut int::myVar = 1; struct::pos (int::x;); variant::v(int; str;); fun int::myFunction()[ myVar = 6; return myVar; ]");
    Parser parser(strStream);
    auto program = parser.parseProgram();

    ASSERT_NE(program, nullptr);

    const auto& functions = program->getFunctions();
    ASSERT_EQ(functions.size(), 1);

    const auto& variables = program->getVariables();
    ASSERT_EQ(variables.size(), 1);

    const auto& structs = program->getStructTypes();
    ASSERT_EQ(structs.size(), 1);

    const auto& variants = program->getVariantTypes();
    ASSERT_EQ(variants.size(), 1);
}

TEST(ParserTest, ParsesProgramFile1) {
    Parser parser(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_file1.txt)");
    auto program = parser.parseProgram();
    ASSERT_NE(program, nullptr);
}

TEST(ParserTest, ParsesProgramFile2) {
    Parser parser(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_file2.txt)");
    auto program = parser.parseProgram();
    ASSERT_NE(program, nullptr);
}

TEST(ParserTest, ParsesProgramFile3) {
    Parser parser(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_file3.txt)");
    auto program = parser.parseProgram();
    ASSERT_NE(program, nullptr);
}






