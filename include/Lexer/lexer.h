#ifndef TKOM_PROJEKT_LEXER_H
#define TKOM_PROJEKT_LEXER_H

#include "token.h"
#include "charReader.h"
#include "myException.h"
#include <cmath>

const auto MAX_NUMBER_LENGTH = 10;

class Lexer {
public:
    CharReader charReader;
    char currChar = ' ';
    Position tokenPos{};
    Token currToken = Token();

public:
    Lexer();
    explicit Lexer(const std::string& file_name);
    explicit Lexer(std::istream& input_stream);
    ~Lexer();

    bool matchAndGetNext(const char& c);
    bool nextChar();
    Token getNextToken();

    // Functions to check and assign currToken
    bool checkAndAssignTokenEOF();
    bool checkAndAssignTokenNumber();
    bool checkAndAssignTokenParenthesis();
    bool checkAndAssignTokenKeywordOrIdentifier();
    bool checkAndAssignTokenSemicolon();
    bool checkAndAssignTokenDoubleColon();
    bool checkAndAssignTokenComma();
    bool checkAndAssignTokenRelationalOperator();
    bool checkAndAssignTokenMathOperator();
    bool checkAndAssignTokenComment();
    bool checkAndAssignTokenDotOrDD();
    bool checkAndAssignTokenStrLiteral();
};

#endif //TKOM_PROJEKT_LEXER_H
