#include <gtest/gtest.h>
#include "lexer.h"


std::string numberLiterals = R"(
# Numbers
int::n = 0;
float::a = 0.0;
float::b = 1.23456;
float::c = -123.123141;
int::d = -123123;
int::e = 123;
int::foobar = 987654321;
float::foobar2 = 0.1234;
float::x1 = 123.;
float::x2 = 0.123;
)";

TEST(Lexer, streamConstructor) {
    std::istringstream input("Hello, Lexer!");
    Lexer lexer(input);
    ASSERT_EQ(lexer.currChar, ' ');
    ASSERT_EQ(lexer.charReader.getCurrChar(), ' ');
    ASSERT_EQ(lexer.charReader.getLine(), 1);
    ASSERT_EQ(lexer.charReader.getColumn(), 0);
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::UNDEF);
    lexer.nextChar();
    ASSERT_EQ(lexer.currChar, 'H');
    ASSERT_EQ(lexer.charReader.getCurrChar(), 'H');
    ASSERT_EQ(lexer.charReader.getLine(), 1);
    ASSERT_EQ(lexer.charReader.getColumn(), 1);
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::UNDEF);
}

TEST(Lexer, fileConstructor) {
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_basic_file.txt)");
    lexer.getNextToken();
    Token token = Token(TokenTypes::FUN_KW, {1, 1});
    ASSERT_EQ(lexer.currToken.getType(), token.getType());
    ASSERT_EQ(lexer.currToken.getPosition().line, token.getPosition().line);
    ASSERT_EQ(lexer.currToken.getPosition().column, token.getPosition().column);
}

TEST(LexerTest, MatchAndGetNextTest) {
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_basic_file.txt)");
    lexer.nextChar(); // Get first char from file_stream
    ASSERT_TRUE(lexer.matchAndGetNext('f')); // Expect a match
    ASSERT_EQ(lexer.currChar, 'u'); // Expect the current character to be 'b'
    ASSERT_FALSE(lexer.matchAndGetNext('a')); // Expect no match
    ASSERT_EQ(lexer.currChar, 'u'); // Current character should still be 'b'
}

TEST(LexerTest, NextCharTest) {
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_basic_file.txt)");
    ASSERT_TRUE(lexer.nextChar()); // Expect success
    ASSERT_EQ(lexer.currChar, 'f'); // Expect the current character to be 'b'
    ASSERT_TRUE(lexer.nextChar()); // Move to the next character
    ASSERT_EQ(lexer.currChar, 'u'); // Expect the current character to be 'c'
    while (lexer.nextChar()) {} // Move to the end of the file
    ASSERT_EQ(lexer.currChar, EOF); // Expect the current character to be EOF
}

TEST(LexerTest, IsEOFTest) {
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_basic_file.txt)");
    for(int i = 0; i < 35; i++) {
        if (i == 5)
            ASSERT_FALSE(lexer.checkAndAssignTokenEOF()); // Expect no EOF
        lexer.nextChar();
    }
    ASSERT_TRUE(lexer.checkAndAssignTokenEOF()); // Expect EOF
}

TEST(LexerTest, IsNumberTest) {
    std::istringstream input("1123 3.14");
    Lexer lexer(input);
    lexer.nextChar(); // Move to first char
    ASSERT_TRUE(lexer.checkAndAssignTokenNumber());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::INT_VALUE);
    lexer.nextChar(); // Move to second number
    ASSERT_TRUE(lexer.checkAndAssignTokenNumber()); // Expect true for a floating-point number
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::FLOAT_VALUE); // Expect the token type to be FLOAT_VALUE
}

TEST(LexerTest, IsNumberStartingWith0Test) {
    std::istringstream input("01123");
    Lexer lexer(input);
    lexer.nextChar(); // Move to first char
    ASSERT_THROW(lexer.checkAndAssignTokenNumber(), MyException);
}

TEST(LexerTest, IsNumber0MessageTest) {
    std::istringstream input("01123");
    Lexer lexer(input);
    lexer.nextChar(); // Move to first char
    lexer.tokenPos = lexer.charReader.getPos(); // Manualy set token pos
    try {
        lexer.checkAndAssignTokenNumber();
        FAIL() << "Expected MyException to be thrown.";
    } catch (const MyException& ex) {
        // Check the exception message here
        std::string expectedErrorMessage = "Number cannot start with 0\n\tat Line: 1, Column: 1\n";
        EXPECT_EQ(ex.what(), expectedErrorMessage);
    } catch (...) {
        FAIL() << "Unexpected exception type.";
    }
}

TEST(LexerTest, IsKeywordOrIdentifierTest) {
    // Test keywords
    std::istringstream input("int float str bool mut fun if else while as return true false struct variant");
    Lexer lexer(input);

    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::INT_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::FLOAT_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::STR_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::BOOL_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::MUT_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::FUN_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::IF_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::ELSE_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::WHILE_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::AS_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::RETURN_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::TRUE_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::FALSE_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::STRUCT_KW);
    lexer.getNextToken();
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::VARIANT_KW);

    // Test identifier
    std::istringstream input2("my_identifier");
    Lexer lexer2 = Lexer(input2);
    lexer2.getNextToken();
    ASSERT_EQ(lexer2.currToken.getType(), TokenTypes::IDENTIFIER);
}

TEST(LexerTest, IsSemicolonTest) {
    std::istringstream input(";");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenSemicolon());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::SEMICOLON);
}

TEST(LexerTest, IsDoubleColonTest) {
    std::istringstream input("::");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenDoubleColon());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::DOUBLE_COLON);
}

TEST(LexerTest, IsCommaTest) {
    std::istringstream input(",");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenComma());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::COMMA);
}

TEST(LexerTest, IsLessThanTest) {
    std::istringstream input("<=");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::LESS_EQUAL);
}

TEST(LexerTest, IsLessThanWithoutEqualTest) {
    std::istringstream input("<");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::LESS);
}

TEST(LexerTest, IsGreaterThanTest) {
    std::istringstream input(">=");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::GREATER_EQUAL);
}

TEST(LexerTest, IsGreaterThanWithoutEqualTest) {
    std::istringstream input(">");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::GREATER);
}

TEST(LexerTest, IsEqualTest) {
    std::istringstream input("==");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::EQUAL);
}

TEST(LexerTest, IsAssignTest) {
    std::istringstream input("=");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::ASSIGN);
}

TEST(LexerTest, IsNotEqualTest) {
    std::istringstream input("!=");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::NOT_EQUAL);
}

TEST(LexerTest, IsNegateTest) {
    std::istringstream input("!");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenRelationalOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::NEGATE);
}

// Test cases for checkAndAssignTokenMathOperator
TEST(LexerTest, IsPlusTest) {
    std::istringstream input("+");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenMathOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::PLUS);
}

TEST(LexerTest, IsMinusTest) {
    std::istringstream input("-");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenMathOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::MINUS);
}

TEST(LexerTest, IsMultiplyTest) {
    std::istringstream input("*");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenMathOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::MULTIPLY);
}

TEST(LexerTest, IsDivideTest) {
    std::istringstream input("/");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenMathOperator());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::DIVIDE);
}

// Test cases for checkAndAssignTokenComment
TEST(LexerTest, IsSingleLineCommentTest) {
    std::istringstream input("# This is a single-line comment\nef");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenComment());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::SINGLE_COMMENT);
}

TEST(LexerTest, IsMultilineCommentTest) {
    std::istringstream input("/# This is a\nmultiline comment #/");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenComment());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::MULTILINE_COMMENT_START);
}

// Test cases for checkAndAssignTokenParenthesis
TEST(LexerTest, IsParenLeftTest) {
    std::istringstream input("(");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenParenthesis());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::PAREN_LEFT);
}

TEST(LexerTest, IsParenRightTest) {
    std::istringstream input(")");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenParenthesis());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::PAREN_RIGHT);
}

TEST(LexerTest, IsBracketLeftTest) {
    std::istringstream input("[");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenParenthesis());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::BRACKET_LEFT);
}

TEST(LexerTest, IsBracketRightTest) {
    std::istringstream input("]");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenParenthesis());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::BRACKET_RIGHT);
}

// Test cases for checkAndAssignTokenDotOrDD
TEST(LexerTest, IsDotTest) {
    std::istringstream input(".");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenDotOrDD());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::DOT);
}

TEST(LexerTest, IsDoubleDotTest) {
    std::istringstream input("..");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenDotOrDD());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::DOUBLE_DOT);
}

// Test cases for checkAndAssignTokenStrLiteral
TEST(LexerTest, IsStrLiteralTest) {
    std::istringstream input("\"Hello, World!\"");
    Lexer lexer(input);
    lexer.nextChar();
    ASSERT_TRUE(lexer.checkAndAssignTokenStrLiteral());
    ASSERT_EQ(lexer.currToken.getType(), TokenTypes::STR_VALUE);
    Token::valueContainer expectedValue = "Hello, World!";
    ASSERT_EQ(lexer.currToken.getValue(), expectedValue);
}

TEST(LexerTest, programReadingTest){
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_file2.txt)");
    std::vector<Token> tokens;
    Token token;
    while (token.getType() != TokenTypes::EOF_TOKEN) {
        token = lexer.getNextToken();
        tokens.push_back(token);
    }
    ASSERT_EQ(tokens.size(), 294);
    ASSERT_EQ(tokens[28].getType(), TokenTypes::ASSIGN);
    ASSERT_EQ(tokens[107].getType(), TokenTypes::SINGLE_COMMENT);
    ASSERT_EQ(tokens[167].getType(), TokenTypes::ASSIGN);
    ASSERT_EQ(tokens[238].getType(), TokenTypes::BRACKET_RIGHT);
}

TEST(LexerTest, programReadingCommentsTest){
    Lexer lexer(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\lexer_test_file3.txt)");
    std::vector<Token> tokens;
    Token token;
    while (token.getType() != TokenTypes::EOF_TOKEN) {
        token = lexer.getNextToken();
        tokens.push_back(token);
    }
    ASSERT_EQ(tokens.size(), 27);
    ASSERT_EQ(tokens[19].getType(), TokenTypes::PAREN_LEFT);
    ASSERT_EQ(tokens[19].getPosition().line, 8);
    ASSERT_EQ(tokens[19].getPosition().column, 14);
    ASSERT_EQ(tokens[20].getType(), TokenTypes::PAREN_RIGHT);
    ASSERT_EQ(tokens[20].getPosition().line, 8);
    ASSERT_EQ(tokens[20].getPosition().column, 15);
}

TEST(LexerTest, NumberLiteralsTest) {
    std::istringstream input(numberLiterals);
    Lexer lexer(input);

    std::vector<Token> tokens;
    Token token;
    while (token.getType() != TokenTypes::EOF_TOKEN) {
        token = lexer.getNextToken();
        tokens.push_back(token);
    }

    ASSERT_EQ(std::get<int>(tokens[5].getValue()), 0);
    ASSERT_FLOAT_EQ(std::get<float>(tokens[17].getValue()), 1.23456);
    ASSERT_FLOAT_EQ(std::get<float>(tokens[23].getValue()), -123.12314);
    ASSERT_EQ(std::get<int>(tokens[29].getValue()), -123123);
    ASSERT_EQ(std::get<int>(tokens[35].getValue()), 123);
    ASSERT_EQ(std::get<int>(tokens[41].getValue()), 987654321);
    ASSERT_FLOAT_EQ(std::get<float>(tokens[47].getValue()), 0.1234);
    ASSERT_FLOAT_EQ(std::get<float>(tokens[53].getValue()), 123.0);
    ASSERT_FLOAT_EQ(std::get<float>(tokens[59].getValue()), 0.123000003);
    // Check for end of file
    ASSERT_EQ(lexer.getNextToken().getType(), TokenTypes::EOF_TOKEN);
}

