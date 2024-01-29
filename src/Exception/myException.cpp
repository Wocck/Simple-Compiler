//
// Created by wojte on 11/16/2023.
//
#include "Exception/myException.h"

MyException::MyException(const std::string& message, struct Position pos) noexcept {
    errorMessage += message;
    errorMessage += "\n\tat Line: ";
    errorMessage += std::to_string(pos.line);
    errorMessage += ", Column: ";
    errorMessage += std::to_string(pos.column);
    errorMessage += "\n";
    this->pos = pos;
}

const char *MyException::what() const noexcept {
    return errorMessage.c_str();
}

struct Position MyException::getPosition() const noexcept {
    return this->pos;
}

MyException::MyException(const std::string &message) noexcept {
    errorMessage += message;
    errorMessage += "\n";
}
