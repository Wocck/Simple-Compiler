#ifndef TKOM_PROJEKT_TOKEN_H
#define TKOM_PROJEKT_TOKEN_H

#include <variant>
#include <string>
#include <optional>
#include "tokenTypes.h"
#include "charReader.h"

class Token {
public:
    typedef std::variant<std::monostate, int, float, std::string> valueContainer;
private:
    TokenTypes tokenType;
    valueContainer tokenValue=0;
    struct Position position{};

public:
    Token();
    Token(TokenTypes, struct Position);
    Token(TokenTypes, std::string, struct Position);
    Token(TokenTypes, int, struct Position);
    Token(TokenTypes, float, struct Position);

    [[nodiscard]] TokenTypes getType() const;
    [[nodiscard]] valueContainer getValue() const;
    [[nodiscard]] Position getPosition() const;
};

#endif //TKOM_PROJEKT_TOKEN_H
