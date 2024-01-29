#ifndef TKOM_PROJEKT_TOKENTYPES_H
#define TKOM_PROJEKT_TOKENTYPES_H

enum class TokenTypes{
    UNDEF,
    EOF_TOKEN,

    // Parentheses
    PAREN_LEFT,
    PAREN_RIGHT,
    BRACKET_LEFT,
    BRACKET_RIGHT,

    // Arithmetic operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    // Comparison operators
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    // Boolean operators
    NEGATE,
    AND,
    OR,

    // Keywords
    IF_KW,
    ELSE_KW,
    WHILE_KW,
    AS_KW,
    RETURN_KW,
    TRUE_KW,
    FALSE_KW,
    FUN_KW,

    // Types Keywords
    INT_KW,
    FLOAT_KW,
    STR_KW,
    BOOL_KW,

    // Type Declaration
    STRUCT_KW,
    VARIANT_KW,
    MUT_KW,

    // Other
    COMMA,
    SEMICOLON,
    DOUBLE_COLON,
    DOT,
    DOUBLE_DOT,
    ASSIGN,
    SINGLE_COMMENT,
    MULTILINE_COMMENT_START,

    // Literals
    INT_VALUE,
    FLOAT_VALUE,
    STR_VALUE,
    IDENTIFIER,
};

constexpr std::string_view strTokenTypes[] =
{
    "UNDEF",
    "EOF_TOKEN",

    // Parentheses
    "PAREN_LEFT",
    "PAREN_RIGHT",
    "BRACKET_LEFT",
    "BRACKET_RIGHT",

    // Arithmetic operators
    "PLUS",
    "MINUS",
    "MULTIPLY",
    "DIVIDE",

    // Comparison operators
    "EQUAL",
    "NOT_EQUAL",
    "LESS",
    "LESS_EQUAL",
    "GREATER",
    "GREATER_EQUAL",

    // Boolean operators
    "NEGATE",
    "AND",
    "OR",

    // Keywords
    "IF_KW",
    "ELSE_KW",
    "WHILE_KW",
    "AS_KW",
    "RETURN_KW",
    "TRUE_KW",
    "FALSE_KW",
    "FUN_KW",

    // Types Keywords
    "INT_KW",
    "FLOAT_KW",
    "STR_KW",
    "BOOL_KW",

    // Type Declaration
    "STRUCT_KW",
    "VARIANT_KW",
    "MUT_KW",

    // Other
    "COMMA",
    "SEMICOLON",
    "DOUBLE_COLON",
    "DOT",
    "DOUBLE_DOT",
    "ASSIGN",
    "SINGLE_COMMENT",
    "MULTILINE_COMMENT_START",

    // Literals
    "INT_VALUE",
    "FLOAT_VALUE",
    "STR_VALUE",
    "IDENTIFIER"
};
#endif //TKOM_PROJEKT_TOKENTYPES_H
