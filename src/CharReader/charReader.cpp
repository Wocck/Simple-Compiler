#include "charReader.h"

CharReader::CharReader() = default;

CharReader::CharReader(std::istream &input_stream) {
    position={1, 0};
    stream = &input_stream;
}

CharReader::CharReader(const std::string &file_name) {
    position={1, 0};
    try {
        file_stream.open(file_name);
    } catch (std::ifstream::failure &e) {
        std::cerr << "Exception opening/reading file: " << file_name << std::endl;
        throw e;
    }
    stream = &file_stream;
}

CharReader::~CharReader() {
    if(file_stream.is_open()) {
        file_stream.close();
    }
}

struct Position CharReader::getPos() const {
    return position;
}

unsigned int CharReader::getLine() const {
    return this->position.line;
}

unsigned int CharReader::getColumn() const {
    return this->position.column;
}

char CharReader::getCurrChar() const {
    return currChar;
}

bool CharReader::nextChar() {
    movePos();
    if (stream) {
        currChar = char(stream->get());
        if (currChar == EOF)
            return false;
        return true;
    }
    return false;
}

void CharReader::movePos() {
    if(!currChar || currChar == EOF)
        return;
    if(currChar == '\n') {
        position.line++;
        position.column = 1;
    } else if (currChar == '\t')
        position.column += TAB_WIDTH - (position.column - 1)% TAB_WIDTH;
    else
        position.column++;
}

char CharReader::peekChar() const {
    if (stream) {
        char c = char(stream->peek());
        if (c == EOF)
            return EOF;
        return c;
    }
    return EOF;
}

