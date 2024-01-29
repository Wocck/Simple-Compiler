#include "parserVisitor.h"

void ParserVisitor::visitBoolLiteral(Nodes::BooleanLiteral *literal) {
    parsed.emplace_back("bool:");
    parsed.emplace_back(literal->getValue() ? "true" : "false");
}

void ParserVisitor::visitIntLiteral(Nodes::IntLiteral *literal) {
    parsed.emplace_back("int:");
    parsed.emplace_back(std::to_string(literal->getValue()));
}

void ParserVisitor::visitFloatLiteral(Nodes::FloatLiteral *literal) {
    parsed.emplace_back("float:");
    parsed.emplace_back(std::to_string(literal->getValue()));
}

void ParserVisitor::visitStringLiteral(Nodes::StringLiteral *literal) {
    parsed.emplace_back("string:");
    parsed.emplace_back(literal->getValue());
}

void ParserVisitor::visitIdentifier(Nodes::Identifier *identifier) {
    parsed.emplace_back("id:");
    parsed.emplace_back(identifier->getName());
}

void ParserVisitor::visitRelOp(Nodes::RelOp *) {

}

void ParserVisitor::visitArtmOp(Nodes::ArtmOp *) {

}

void ParserVisitor::visitFactorOp(Nodes::FactorOp *) {

}

void ParserVisitor::visitUnaryOp(Nodes::UnaryOp *) {

}

void ParserVisitor::visitCastOp(Nodes::CastOp *) {

}

void ParserVisitor::visitCastingExpr(Nodes::CastingExpr *) {

}

void ParserVisitor::visitUnaryExpr(Nodes::UnaryExpr *) {

}

void ParserVisitor::visitMulExpr(Nodes::MulExpr *) {

}

void ParserVisitor::visitArtmExpr(Nodes::ArtmExpr *) {

}

void ParserVisitor::visitRelExpr(Nodes::RelExpr *) {

}

void ParserVisitor::visitAndExpr(Nodes::AndExpr *) {

}

void ParserVisitor::visitOrExpr(Nodes::OrExpr *) {

}

void ParserVisitor::visitExpr(Nodes::Expression *) {

}

void ParserVisitor::visitFuncCall(Nodes::FunCall *) {

}

void ParserVisitor::visitVariableRef(Nodes::VarReference *) {

}

void ParserVisitor::visitDeclaration(Nodes::Declaration *) {

}

void ParserVisitor::visitType(Nodes::Type *) {

}

void ParserVisitor::visitTypeDecl(Nodes::TypeDecl *) {

}

void ParserVisitor::visitVariableDeclaration(Nodes::VariableDeclaration *) {

}

void ParserVisitor::visitStructTypeDefinition(Nodes::StructTypeDefinition *) {

}

void ParserVisitor::visitStructVarDeclaration(Nodes::StructVarDeclaration *) {

}

void ParserVisitor::visitVariantTypeDefinition(Nodes::VariantTypeDefinition *) {

}

void ParserVisitor::visitVariantVarDeclaration(Nodes::VariantVarDeclaration *) {

}

void ParserVisitor::visitAssignment(Nodes::Assignment *) {

}

void ParserVisitor::visitStructFieldAssignment(Nodes::StructFieldAssignment *) {

}

void ParserVisitor::visitReturnStatement(Nodes::ReturnStatement *) {

}

void ParserVisitor::visitBlock(Nodes::Block *) {

}

void ParserVisitor::visitIfStatement(Nodes::IfStatement *) {

}

void ParserVisitor::visitWhileStatement(Nodes::WhileStatement *) {

}

void ParserVisitor::visitFunctionCallStatement(Nodes::FunctionCallStatement *) {

}

void ParserVisitor::visitFunctionDeclaration(Nodes::FunctionDeclaration *) {

}

void ParserVisitor::visitProgram(Nodes::Program *) {

}
