#ifndef SingleCharLexerRule_H
#define SingleCharLexerRule_H
#include "LexerRule.hpp"
#include <Typedefs.hpp>

struct SingleCharLexerRule : LexerRule {
    SingleCharLexerRule(size_t type, const MathLib::Sequence<char>& chars);
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override;

    private:
    MathLib::Array<char> chars;
};


#endif