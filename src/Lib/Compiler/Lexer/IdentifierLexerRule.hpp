#ifndef MathLib_Compiler_Lexer_IdentifierRule_H
#define MathLib_Compiler_Lexer_IdentifierRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct IdentifierLexerRule : LexerRule {
        IdentifierLexerRule(size_t type, bool startWithDigit);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;

        private:
        bool startWithDigit;
    };
}

#endif