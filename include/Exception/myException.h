#ifndef TKOM_PROJEKT_MYEXCEPTION_H
#define TKOM_PROJEKT_MYEXCEPTION_H

#include <string>
#include <utility>
#include "charReader.h"

class MyException : public std::exception{
public:
    MyException() = default;
    MyException(const std::string& message, struct Position pos) noexcept;
    explicit MyException(const std::string& message) noexcept;

    [[nodiscard]] const char * what() const noexcept override;

    [[nodiscard]] struct Position getPosition() const noexcept;

private:
    std::string errorMessage;
    struct Position pos{};
};

#endif //TKOM_PROJEKT_MYEXCEPTION_H
