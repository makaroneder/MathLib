#ifndef MathLib_Compiler_Lexer_DigitRule_H
#define MathLib_Compiler_Lexer_DigitRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct DigitLexerRule : LexerRule {
        DigitLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif