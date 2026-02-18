#ifndef MathLib_Compiler_Lexer_WhitespaceRule_H
#define MathLib_Compiler_Lexer_WhitespaceRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct WhitespaceLexerRule : LexerRule {
        WhitespaceLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif