#ifndef IdentifierLexerRule_H
#define IdentifierLexerRule_H
#include "LexerRule.hpp"

struct IdentifierLexerRule : LexerRule {
    IdentifierLexerRule(size_t type);
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override;
};

#endif