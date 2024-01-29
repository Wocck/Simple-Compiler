cmake_minimum_required(VERSION 3.26)
project(tkom_projekt)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_subdirectory(tests)

add_library(compiler_lib
        include/Parser/parser.h
        include/Parser/syntaxTree.h
        include/Visitors/syntaxTreeVisitor.h
        include/Visitors/visitorTemplate.h
        include/Visitors/parserVisitor.h
        include/Visitors/semanticVisitor.h
        include/Visitors/interpreterVisitor.h
        include/CharReader/charReader.h
        include/Lexer/lexer.h
        include/Lexer/token.h
        include/Lexer/tokenTypes.h
        include/Parser/symbolTable.h
        include/Parser/symbolTableManager.h
        include/Exception/myException.h)

target_include_directories(compiler_lib
        PUBLIC
                include/CharReader
                include/Lexer
                include/Parser
                include/Visitors
                include/Exception)
target_sources(compiler_lib
        PRIVATE
                src/CharReader/charReader.cpp
                src/Lexer/token.cpp
                src/Lexer/lexer.cpp
                src/Parser/parser.cpp
                src/Parser/syntaxTree.cpp
                src/Visitors/syntaxTreeVisitor.cpp
                src/Visitors/visitorTemplate.cpp
                src/Visitors/parserVisitor.cpp
                src/Visitors/semanticVisitor.cpp
                src/Visitors/interpreterVisitor.cpp
                src/Parser/symbolTable.cpp
                src/Parser/symbolTableManager.cpp
                src/Exception/myException.cpp)

add_executable(tkom_projekt
        include/CharReader/charReader.h
        include/Lexer/lexer.h
        include/Lexer/token.h
        src/CharReader/charReader.cpp
        src/Lexer/lexer.cpp
        src/Lexer/token.cpp
        src/main.cpp
        include/Lexer/tokenTypes.h
        include/Exception/myException.h
        src/Exception/myException.cpp
        include/Visitors/syntaxTreeVisitor.h
        src/Visitors/syntaxTreeVisitor.cpp)
target_link_libraries(tkom_projekt compiler_lib)
