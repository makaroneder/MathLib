#ifndef MathLib_Compiler_Lexer_SingleCharRule_H
#define MathLib_Compiler_Lexer_SingleCharRule_H
#include "LexerRule.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    struct SingleCharLexerRule : LexerRule {
        SingleCharLexerRule(size_t type, const Sequence<char>& chars);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;

        private:
        Array<char> chars;
    };
}

#endif