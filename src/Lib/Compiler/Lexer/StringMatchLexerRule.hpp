#ifndef MathLib_Compiler_Lexer_StringMatchRule_H
#define MathLib_Compiler_Lexer_StringMatchRule_H
#include "LexerRule.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    struct StringMatchLexerRule : LexerRule {
        StringMatchLexerRule(size_t type, const Sequence<char>& value);
        StringMatchLexerRule(size_t type, const Sequence<String>& value);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;

        private:
        Array<String> value;
    };
}

#endif