#ifndef TokenType_H
#define TokenType_H
#include <stdint.h>

enum class TokenType : uint8_t {
    Identifier,
    Semicolon,
    Comma,
    ParenthesesStart,
    ParenthesesEnd,
    BracketsStart,
    BracketsEnd,

    Scope,
    Function,
    Return,
    Let,
};

#endif