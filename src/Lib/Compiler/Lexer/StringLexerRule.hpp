#ifndef MathLib_Compiler_Lexer_StringRule_H
#define MathLib_Compiler_Lexer_StringRule_H
#include "LexerRule.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    struct StringLexerRule : LexerRule {
        StringLexerRule(size_t type, const Sequence<char>& value);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;

        private:
        String value;
    };
}

#endif