#ifndef MathLib_Compiler_Lexer_CommentRule_H
#define MathLib_Compiler_Lexer_CommentRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct CommentLexerRule : LexerRule {
        CommentLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif