#include <gtest/gtest.h>
#include "myException.h"


TEST(MyException, ConstructorAndWhat) {
    Position pos{1, 2};
    std::string message = "Test error message";
    MyException exception(message, pos);

    // Test that the message returned by what() matches the expected message.
    ASSERT_STREQ(exception.what(), "Test error message\n\tat Line: 1, Column: 2\n");

    // Test getPosition() to ensure it returns the correct Position.
    ASSERT_EQ(exception.getPosition().line, pos.line);
    ASSERT_EQ(exception.getPosition().column, pos.column);
}

TEST(MyException, EmptyConstructorAndWhat) {
    MyException exception;

    // Test that the default message is empty.
    ASSERT_STREQ(exception.what(), "");

    // Test getPosition() with the default Position.
    ASSERT_EQ(exception.getPosition().line, 0);
    ASSERT_EQ(exception.getPosition().column, 0);
}

TEST(MyException, MessageAndPosition) {
    Position pos{3, 4};
    std::string message = "Another error message";
    MyException exception(message, pos);

    // Test that the message and Position match the expected values.
    ASSERT_STREQ(exception.what(), "Another error message\n\tat Line: 3, Column: 4\n");
    ASSERT_EQ(exception.getPosition().line, pos.line);
    ASSERT_EQ(exception.getPosition().column, pos.column);
}
