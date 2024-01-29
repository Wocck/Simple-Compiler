include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(allTests
        charReader_test.cpp
        lexer_test.cpp
        token_test.cpp
        myException_test.cpp
        syntaxTree_test.cpp
        parser_test.cpp
        semanticAnalyzer_test.cpp
        interpreter_test.cpp
        ../../include/Visitors/syntaxTreeVisitor.h
        ../../src/Visitors/syntaxTreeVisitor.cpp)

add_executable(charReaderTests charReader_test.cpp)
add_executable(lexerTests lexer_test.cpp)
add_executable(tokenTests token_test.cpp)
add_executable(myExceptionTests myException_test.cpp)
add_executable(syntaxTreeTests syntaxTree_test.cpp)
add_executable(parserTests parser_test.cpp)
add_executable(semanticTests semanticAnalyzer_test.cpp)
add_executable(interpreterTests interpreter_test.cpp)

target_link_libraries(allTests gtest gtest_main compiler_lib)
target_link_libraries(charReaderTests gtest gtest_main compiler_lib)
target_link_libraries(lexerTests  gtest gtest_main compiler_lib)
target_link_libraries(tokenTests gtest gtest_main compiler_lib)
target_link_libraries(myExceptionTests gtest gtest_main compiler_lib)
target_link_libraries(syntaxTreeTests gtest gtest_main compiler_lib)
target_link_libraries(parserTests gtest gtest_main compiler_lib)
target_link_libraries(semanticTests gtest gtest_main compiler_lib)
target_link_libraries(interpreterTests gtest gtest_main compiler_lib)