#include "LexerRule.hpp"

LexerRule::LexerRule(size_t type) : type(type) {}
size_t LexerRule::GetType(void) const {
    return type;
}