#include "token.h"

Token::Token()
    : tokenType(TokenTypes::UNDEF), position({0, 0}){
}

Token::Token(TokenTypes type, struct Position pos)
    : tokenType(type), position(pos){
}

Token::Token(TokenTypes type, std::string lexeme, struct Position pos)
    : tokenType(type), position(pos), tokenValue(std::move(lexeme)){
}

Token::Token(TokenTypes type, int value, Position pos)
    : tokenType(type), tokenValue(value), position(pos) {
}

Token::Token(TokenTypes type, float value, Position pos)
    : tokenType(type), tokenValue(value), position(pos) {
}

TokenTypes Token::getType() const {
    return tokenType;
}

Token::valueContainer Token::getValue() const {
        return tokenValue;
}

struct Position Token::getPosition() const {
    return position;
}






