#ifndef MathLib_Compiler_Lexer_FloatRule_H
#define MathLib_Compiler_Lexer_FloatRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct FloatLexerRule : LexerRule {
        FloatLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif