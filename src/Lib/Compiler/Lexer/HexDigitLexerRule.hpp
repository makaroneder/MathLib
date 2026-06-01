#ifndef MathLib_Compiler_Lexer_HexDigitRule_H
#define MathLib_Compiler_Lexer_HexDigitRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct HexDigitLexerRule : LexerRule {
        HexDigitLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif