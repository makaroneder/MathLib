#ifndef WhitespaceLexerRule_H
#define WhitespaceLexerRule_H
#include "LexerRule.hpp"

struct WhitespaceLexerRule : LexerRule {
    WhitespaceLexerRule(size_t type);
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override;
};

#endif