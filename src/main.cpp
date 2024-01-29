#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "semanticVisitor.h"
#include "interpreterVisitor.h"

std::string ex1 = "fun int::main()[ int::number = 29; if number [ print(5); ] return 1; ]";
std::string ex2 = "# testing string escaping\n"
                  "fun int::main()[\n"
                  "    str::s = \"ala\\nma\\nkota\";\n"
                  "    str::t = \"k\\t\\tala\";\n"
                  "    str::r = \"\\\\c\\\\b\";\n"
                  "    str::u = \"\n\\t\\x\";\n"
                  "\n"
                  "    print(s);\n"
                  "    print(t);\n"
                  "    print(r);\n"
                  "    print(u);\n"
                  "]";
std::string ex3 = "# Function to check if a number is prime\n"
                  "fun bool::is_prime(int::number)[\n"
                  "    if number < 2 [\n"
                  "        return false;\n"
                  "    ]\n"
                  "    mut int::i = 2;\n"
                  "    while i < number [\n"
                  "        i = i + 1;\n"
                  "    ]\n"
                  "    return true;\n"
                  "]\n"
                  "\n"
                  "# Function to sum two numbers\n"
                  "fun int::sum(int::a, int::b)[\n"
                  "    return a + b;\n"
                  "]\n"
                  "\n"
                  "# Main function\n"
                  "fun int::main()[\n"
                  "    int::num = 11;\n"
                  "    bool::prime = is_prime(num);\n"
                  "    if prime [\n"
                  "        print(num, \" is prime.\");\n"
                  "    ] else [\n"
                  "        print(num, \" is not prime.\");\n"
                  "    ]\n"
                  "\n"
                  "    int::result = sum(10, 5);\n"
                  "    print(\"Sum of 10 and 5 is \", result);\n"
                  "\n"
                  "    return 0;\n"
                  "]";


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -f <file_path> or -s <string>" << std::endl;
        return 1;
    }
    std::string argType = argv[1];
    std::string argValue = argv[2];
    std::istringstream strStream;
    Lexer* lexer;
    Parser* parser;
    if (argType == "-f") {
        std::cout << "File path provided: " << argValue << std::endl;
        lexer = new Lexer(argValue);
        parser = new Parser(argValue);
    } else if (argType == "-s") {
        std::cout << "String provided: " << argValue << std::endl;
        // here change example provided
        strStream.str(ex3);
        lexer = new Lexer(strStream);
        parser = new Parser(strStream);
    } else {
        std::cerr << "Invalid argument. Use -f for file path or -s for string." << std::endl;
        return 1;
    }
    try {
        std::unique_ptr<Nodes::Program> program = std::move(parser->parseProgram());
        SemanticVisitor semanticVisitor(program->getStructTypes(), program->getVariantTypes());
        program->accept(semanticVisitor);
        InterpreterVisitor interpreterVisitor(program->getStructTypes(), program->getVariantTypes());
        program->accept(interpreterVisitor);
    }
    catch (MyException &e) {
        std::cout << e.what();
    }
    return 0;

}