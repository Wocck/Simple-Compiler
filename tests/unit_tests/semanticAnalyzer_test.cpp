#include <gtest/gtest.h>
#include <sstream>

#include "interpreterVisitor.h"
#include "parser.h"
#include "lexer.h"
#include "semanticVisitor.h"

std::string test1 = "fun int::main() [ int::a = 5; int::b = 10; int::c = a + b; print(c); ]";

TEST(InterpreterTest, test1) {
    std::istringstream strStream;
    strStream.str(test1);
    Lexer lexer(strStream);
    Parser parser(strStream);
    std::unique_ptr<Nodes::Program> program = std::move(parser.parseProgram());
    InterpreterVisitor interpreterVisitor(program->getStructTypes(), program->getVariantTypes());
    program->accept(interpreterVisitor);
}

std::string testPrimeCheck = "fun bool::is_prime(int::number)[] fun int::main()[return 1;]";

TEST(SemanticAnalyzerTest, BasicFunctionCallAndPrimeCheck) {
    std::istringstream strStream;
    strStream.str(testPrimeCheck);
    Lexer lexer(strStream);
    Parser parser(strStream);
    try {
        std::unique_ptr<Nodes::Program> program = std::move(parser.parseProgram());
        SemanticVisitor semanticVisitor(program->getStructTypes(), program->getVariantTypes());
        program->accept(semanticVisitor);
        SUCCEED();
    } catch (MyException &e) {
        FAIL() << e.what();
    }
}

std::string testStructVariant = "struct::Position(int::x; float::y;); variant::User_input(int; float;); fun int::main() []";

TEST(SemanticAnalyzerTest, StructAndVariantUsage) {
    std::istringstream strStream;
    strStream.str(testStructVariant);
    Lexer lexer(strStream);
    Parser parser(strStream);
    try {
        std::unique_ptr<Nodes::Program> program = std::move(parser.parseProgram());
        SemanticVisitor semanticVisitor(program->getStructTypes(), program->getVariantTypes());
        program->accept(semanticVisitor);
        SUCCEED();
    } catch (MyException &e) {
        FAIL() << e.what();
    }
}

std::string testTypeMismatch = "fun int::main() [ int::number = true; ]";

TEST(SemanticAnalyzerTest, TypeMismatch) {
    std::istringstream strStream;
    strStream.str(testTypeMismatch);
    Lexer lexer(strStream);
    Parser parser(strStream);
    std::unique_ptr<Nodes::Program> program = std::move(parser.parseProgram());
    SemanticVisitor semanticVisitor(program->getStructTypes(), program->getVariantTypes());
    program->accept(semanticVisitor);
    //ASSERT_THROW({}, MyException);
}

