#ifndef DigitLexerRule_H
#define DigitLexerRule_H
#include "LexerRule.hpp"

struct DigitLexerRule : LexerRule {
    DigitLexerRule(size_t type);
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override;
};

#endif