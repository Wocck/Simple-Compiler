#include <map>
#include "lexer.h"

static const std::map<std::string, TokenTypes> keywordMap = {
        {"int", TokenTypes::INT_KW},
        {"float", TokenTypes::FLOAT_KW},
        {"str", TokenTypes::STR_KW},
        {"bool", TokenTypes::BOOL_KW},
        {"mut", TokenTypes::MUT_KW},
        {"fun", TokenTypes::FUN_KW},
        {"if", TokenTypes::IF_KW},
        {"else", TokenTypes::ELSE_KW},
        {"else", TokenTypes::ELSE_KW},
        {"while", TokenTypes::WHILE_KW},
        {"as", TokenTypes::AS_KW},
        {"return", TokenTypes::RETURN_KW},
        {"true", TokenTypes::TRUE_KW},
        {"false", TokenTypes::FALSE_KW},
        {"struct", TokenTypes::STRUCT_KW},
        {"variant", TokenTypes::VARIANT_KW},
        {"or", TokenTypes::OR},
        {"and", TokenTypes::AND},
};

Lexer::Lexer() : charReader(){}

Lexer::Lexer(const std::string &file_name) : charReader(file_name){}

Lexer::Lexer(std::istream &input_stream) : charReader(input_stream){}

Lexer::~Lexer() = default;

bool Lexer::matchAndGetNext(const char& c){
    if(currChar == c) {
        nextChar();
        return true;
    }
    return false;
}

bool Lexer::nextChar() {
    charReader.nextChar();
    currChar = charReader.getCurrChar();
    if(currChar == EOF)
        return false;
    return currChar;
}

Token Lexer::getNextToken(){
    while (isspace(currChar)) {
        nextChar();
    }
    tokenPos = charReader.getPos();

    currToken = Token(TokenTypes::UNDEF, tokenPos);
    if (
            checkAndAssignTokenEOF() ||
            checkAndAssignTokenComment() ||
            checkAndAssignTokenKeywordOrIdentifier() ||
            checkAndAssignTokenDoubleColon() ||
            checkAndAssignTokenDotOrDD() ||
            checkAndAssignTokenParenthesis() ||
            checkAndAssignTokenMathOperator() ||
            checkAndAssignTokenRelationalOperator() ||
            checkAndAssignTokenNumber() ||
            checkAndAssignTokenSemicolon() ||
            checkAndAssignTokenComma() ||
            checkAndAssignTokenStrLiteral()){}
    else {
        while (!isspace(currChar) && nextChar()) {}
    }
    return currToken;
}

bool Lexer::checkAndAssignTokenEOF() {
    if (currChar == EOF) {
        currToken = Token(TokenTypes::EOF_TOKEN, tokenPos);
        return true;
    }
    return false;
}

bool Lexer::checkAndAssignTokenNumber() {
    if (!isdigit(currChar) && currChar != '-')
        return false;
    if (currChar == '-' && !isdigit(charReader.peekChar()))
        return false;


    std::string numberStr;
    if (currChar == '-') {
        numberStr += currChar;
        nextChar();
    }

    // Accumulate digits before the decimal point
    while (isdigit(currChar)) {
        numberStr += currChar;
        nextChar();
    }

    bool isFloat = false;
    // Check for decimal point and accumulate digits after it
    if (currChar == '.') {
        isFloat = true;
        numberStr += currChar;
        nextChar();
        while (isdigit(currChar)) {
            numberStr += currChar;
            nextChar();
        }
    }

    // Convert the string to a number token
    try {
        if (isFloat) {
            float floatValue = std::stof(numberStr);
            currToken = Token(TokenTypes::FLOAT_VALUE, floatValue, tokenPos);
        } else {
            if (numberStr.length() > 1 && numberStr[0] == '0')
                throw MyException("Number cannot start with 0", tokenPos);
            int intValue = std::stoi(numberStr);
            currToken = Token(TokenTypes::INT_VALUE, intValue, tokenPos);
        }
    } catch (const std::invalid_argument& e) {
        throw MyException("Invalid number format", tokenPos);
    } catch (const std::out_of_range& e) {
        throw MyException("Number too long", tokenPos);
    }

    return true;
}

bool Lexer::checkAndAssignTokenParenthesis() {
    if (matchAndGetNext('('))
    {
        currToken = Token(TokenTypes::PAREN_LEFT, tokenPos);
        return true;
    }
    else if (matchAndGetNext(')'))
    {
        currToken = Token(TokenTypes::PAREN_RIGHT, tokenPos);
        return true;
    }
    else if (matchAndGetNext('['))
    {
        currToken = Token(TokenTypes::BRACKET_LEFT, tokenPos);
        return true;
    }
    else if (matchAndGetNext(']'))
    {
        currToken = Token(TokenTypes::BRACKET_RIGHT, tokenPos);
        return true;
    }
    return false;
}

bool Lexer::checkAndAssignTokenKeywordOrIdentifier() {
    if (!isalpha(currChar) && currChar != '_')
        return false;

    std::string lexeme;
    lexeme += currChar;
    nextChar();
    while (isalnum(currChar) || currChar == '_')
    {
        lexeme += currChar;
        nextChar();
    }

    auto it = keywordMap.find(lexeme);
    if (it != keywordMap.end()) {
        currToken = Token(it->second, tokenPos);
        return true;
    }
    currToken = Token(TokenTypes::IDENTIFIER, lexeme, tokenPos);
    return true;
}

bool Lexer::checkAndAssignTokenSemicolon() {
    if (matchAndGetNext(';'))
    {
        currToken = Token(TokenTypes::SEMICOLON, tokenPos);
        return true;
    }
    return false;
}

bool Lexer::checkAndAssignTokenDoubleColon() {
    if (matchAndGetNext(':'))
    {
        if (matchAndGetNext(':'))
        {
            currToken = Token(TokenTypes::DOUBLE_COLON, tokenPos);
            return true;
        }
        else
        {
            throw MyException("Expected ':'", tokenPos);
        }
    }
    return false;
}

bool Lexer::checkAndAssignTokenComma() {
    if (matchAndGetNext(','))
    {
        currToken = Token(TokenTypes::COMMA, tokenPos);
        return true;
    }
    return false;
}

bool Lexer::checkAndAssignTokenRelationalOperator() {
    if (matchAndGetNext('<')){
        if (matchAndGetNext('=')){
            currToken = Token(TokenTypes::LESS_EQUAL, tokenPos);
            return true;
        }
        else{
            currToken = Token(TokenTypes::LESS, tokenPos);
            return true;
        }
    }
    if (matchAndGetNext('>')){
        if (matchAndGetNext('=')){
            currToken = Token(TokenTypes::GREATER_EQUAL, tokenPos);
            return true;
        }
        else{
            currToken = Token(TokenTypes::GREATER, tokenPos);
            return true;
        }
    }
    if (matchAndGetNext('=')){
        if (matchAndGetNext('=')){
            currToken = Token(TokenTypes::EQUAL, tokenPos);
            return true;
        }
        else {
            currToken = Token(TokenTypes::ASSIGN, tokenPos);
            return true;
        }
    }
    if (matchAndGetNext('!')) {
        if (matchAndGetNext('=')) {
            currToken = Token(TokenTypes::NOT_EQUAL, tokenPos);
            return true;
        } else {
            currToken = Token(TokenTypes::NEGATE, tokenPos);
            return true;
        }
    }
    return false;
}

bool Lexer::checkAndAssignTokenMathOperator() {
    if (matchAndGetNext('+')){
        currToken = Token(TokenTypes::PLUS, tokenPos);
        return true;
    }
    if (matchAndGetNext('-')){
        currToken = Token(TokenTypes::MINUS, tokenPos);
        return true;
    }
    if (matchAndGetNext('*')){
        currToken = Token(TokenTypes::MULTIPLY, tokenPos);
        return true;
    }
    if (matchAndGetNext('/')){
        currToken = Token(TokenTypes::DIVIDE, tokenPos);
        return true;
    }
    return false;
}

bool Lexer::checkAndAssignTokenComment() {
    if (matchAndGetNext('#')){
        while (currChar != '\n' && currChar != EOF){
            nextChar();
        }
        currToken = Token(TokenTypes::SINGLE_COMMENT, tokenPos);
        return true;
    }
    if (matchAndGetNext('/')){
        if (matchAndGetNext('#')){
            currToken = Token(TokenTypes::MULTILINE_COMMENT_START, tokenPos);
            while (currChar != EOF){
                if (matchAndGetNext('#')){
                    if (matchAndGetNext('/')) {
                        return true;
                    }
                    continue;
                }
                nextChar();
            }
            throw MyException("Expected '#/'", tokenPos);
        }
        // w przeciwnym wypadku to musi być slash
        currToken = Token(TokenTypes::DIVIDE, tokenPos);
    }
    return false;
}

bool Lexer::checkAndAssignTokenDotOrDD() {
    if (matchAndGetNext('.'))
    {
        if (isdigit(currChar))
        {
            throw MyException("Number cannot start with .", tokenPos);
        }
        if (matchAndGetNext('.'))
        {
            currToken = Token(TokenTypes::DOUBLE_DOT, tokenPos);
            return true;
        }
        else
        {
            currToken = Token(TokenTypes::DOT, tokenPos);
            return true;
        }
    }
    return false;
}

bool Lexer::checkAndAssignTokenStrLiteral() {
    if (matchAndGetNext('"')){
        std::string lexeme;
        while (currChar != '"'){
            if (currChar == '\\'){
                nextChar();
                switch (currChar) {
                    case 'n': lexeme += '\n'; break; // Nowa linia
                    case 't': lexeme += '\t'; break; // Tabulacja
                    case '\\': lexeme += '\\'; break; // Znak wsteczny
                    case '"': lexeme += '"'; break;
                    default: lexeme += '\\'; lexeme += currChar; break;
                }
            } else {
                lexeme += currChar;
            }
            nextChar();
        }
        nextChar();
        currToken = Token(TokenTypes::STR_VALUE, lexeme, tokenPos);
        return true;
    }
    return false;
}
