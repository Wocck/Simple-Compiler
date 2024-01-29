// charReader.h
#ifndef TKOM_PROJEKT_CHARREADER_H
#define TKOM_PROJEKT_CHARREADER_H

#include <iostream>
#include <fstream>

const auto TAB_WIDTH = 4;

struct Position{
    unsigned int line;
    unsigned int column;
};

class CharReader {
public:
    CharReader();
    explicit CharReader(std::istream& input_stream);
    explicit CharReader(const std::string& file_name);
    ~CharReader();

    Position getPos() const;
    unsigned int getLine() const;
    unsigned int getColumn() const;
    char getCurrChar() const;
    bool nextChar();
    void movePos();
    char peekChar() const;

private:
    std::istream* stream=nullptr;
    std::ifstream file_stream;
    Position position{};
    char currChar=' ';

};

#endif //TKOM_PROJEKT_CHARREADER_H
