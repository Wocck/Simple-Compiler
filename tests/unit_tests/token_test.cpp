#include <gtest/gtest.h>
#include <Lexer/token.h>

TEST(Token, emptyConstructor) {
    Token token;
    ASSERT_EQ(token.getType(), TokenTypes::UNDEF);
    ASSERT_EQ(token.getPosition().line, 0);
    ASSERT_EQ(token.getPosition().column, 0);
    Token::valueContainer value = token.getValue();
    ASSERT_TRUE(std::holds_alternative<int>(value));
    ASSERT_EQ(std::get<int>(value), 0);
}

TEST(Token, basic) {
    Position pos{1, 1};
    TokenTypes type = TokenTypes::INT_KW;
    Token token = Token(type, pos);
    ASSERT_EQ(token.getType(), type);
    ASSERT_EQ(token.getPosition().line, pos.line);
    ASSERT_EQ(token.getPosition().column, pos.column);
}

TEST(Token, basicStr) {
    std::string id = "foobar";
    Position pos{1, 1};
    TokenTypes type = TokenTypes::IDENTIFIER;
    Token token = Token(type, id, pos);
    ASSERT_EQ(token.getType(), type);
    ASSERT_EQ(token.getPosition().line, pos.line);
    ASSERT_EQ(token.getPosition().column, pos.column);
    Token::valueContainer value = token.getValue();
    ASSERT_EQ(std::get<std::string>(value), id);
}

TEST(Token, basicInt) {
    int number = 3;
    Position pos{1, 1};
    TokenTypes type = TokenTypes::INT_VALUE;
    Token token = Token(type, number, pos);
    ASSERT_EQ(token.getType(), type);
    ASSERT_EQ(token.getPosition().line, pos.line);
    ASSERT_EQ(token.getPosition().column, pos.column);
    Token::valueContainer value = token.getValue();
    ASSERT_EQ(std::get<int>(value), number);
}

TEST(Token, basicFloat) {
    float number = 3.1;
    Position pos{1, 1};
    TokenTypes type = TokenTypes::FLOAT_VALUE;
    Token token = Token(type, number, pos);
    ASSERT_EQ(token.getType(), type);
    ASSERT_EQ(token.getPosition().line, pos.line);
    ASSERT_EQ(token.getPosition().column, pos.column);
    Token::valueContainer value = token.getValue();
    ASSERT_EQ(std::get<float>(value), number);
}

TEST(Token, copyConstructor) {
    Position pos{1, 1};
    TokenTypes type = TokenTypes::IDENTIFIER;
    Token originalToken(type, "foo", pos);

    Token copiedToken(originalToken);
    ASSERT_EQ(copiedToken.getType(), originalToken.getType());
    ASSERT_EQ(copiedToken.getPosition().line, originalToken.getPosition().line);
    ASSERT_EQ(copiedToken.getPosition().column, originalToken.getPosition().column);
    Token::valueContainer copiedValue = copiedToken.getValue();
    Token::valueContainer originalValue = originalToken.getValue();
    ASSERT_EQ(std::get<std::string>(copiedValue), std::get<std::string>(originalValue));
}

TEST(Token, typeCheck) {
    Position pos{1, 1};
    TokenTypes type = TokenTypes::FLOAT_VALUE;
    Token token(type, 3.14f, pos);

    ASSERT_TRUE(std::holds_alternative<float>(token.getValue()));
    ASSERT_FALSE(std::holds_alternative<int>(token.getValue()));
    ASSERT_FALSE(std::holds_alternative<std::string>(token.getValue()));
}