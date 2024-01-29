#include <gtest/gtest.h>
#include <sstream>
#include "charReader.h"

TEST(CharReader, streamConstructor) {
    std::istringstream input("Hello, CharReader!");
    CharReader reader(input);
    ASSERT_EQ(reader.getLine(), 1);
    ASSERT_EQ(reader.getColumn(), 0);
    ASSERT_EQ(reader.getCurrChar(), ' ');
    reader.nextChar();
    ASSERT_EQ(reader.getLine(), 1);
    ASSERT_EQ(reader.getColumn(), 1);
    ASSERT_EQ(reader.getCurrChar(), 'H');
}

TEST(CharReader, fileConstructor) {
    CharReader reader(R"(C:\Users\wojte\CLionProjects\tkom-projekt\tests\test_data\charReader_test_file1.txt)");
    ASSERT_EQ(reader.getLine(), 1);
    ASSERT_EQ(reader.getColumn(), 0);
    reader.nextChar();
    ASSERT_EQ(reader.getCurrChar(), 'F');
}

TEST(CharReader, nextChar) {
    std::istringstream input("Hello, CharReader!");
    CharReader reader(input);
    reader.nextChar();
    ASSERT_EQ(reader.getCurrChar(), 'H');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'e');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'l');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'l');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'o');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), ',');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), ' ');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'C');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'h');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'a');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'r');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'R');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'e');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'a');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'd');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'e');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), 'r');
    ASSERT_TRUE(reader.nextChar());
    ASSERT_EQ(reader.getCurrChar(), '!');
    ASSERT_FALSE(reader.nextChar()); // End of stream
}

TEST(CharReaderTest, MovePosTest) {
    CharReader charReader("Hello, World!\n\tThis is a test.ewcewcewcwecew wedewfewf");

    // Test moving Position for regular characters
    charReader.movePos(); // Current Position: Line 1, Column 1
    ASSERT_EQ(charReader.getLine(), 1);
    ASSERT_EQ(charReader.getColumn(), 1);

    charReader.nextChar(); // Advance to the next character (e.g., 'a')
    charReader.movePos(); // Current Position: Line 1, Column 2
    ASSERT_EQ(charReader.getLine(), 1);
    ASSERT_EQ(charReader.getColumn(), 2);

}

TEST(CharReaderTest, NextCharTest) {
    std::istringstream input("He\n\tThis is a test.");
    CharReader charReader(input);

    // Test reading characters from the stream
    charReader.nextChar(); // Read 'H'
    ASSERT_EQ(charReader.getCurrChar(), 'H');

    charReader.nextChar(); // Read 'e'
    ASSERT_EQ(charReader.getCurrChar(), 'e');

    // Test moving to the next line
    charReader.nextChar(); // Read '\n'
    ASSERT_EQ(charReader.getCurrChar(), '\n');

    // Test reading tabs
    charReader.nextChar(); // Read '\t'
    ASSERT_EQ(charReader.getCurrChar(), '\t');

    // Test reading until EOF
    while (charReader.nextChar()) {} // Read until EOF
    ASSERT_EQ(charReader.getCurrChar(), EOF);
}
