#ifndef MathLib_Compiler_Lexer_MultiLineCommentRule_H
#define MathLib_Compiler_Lexer_MultiLineCommentRule_H
#include "LexerRule.hpp"

namespace MathLib {
    struct MultiLineCommentLexerRule : LexerRule {
        MultiLineCommentLexerRule(size_t type);
        virtual void Match(const Sequence<char>& str, size_t& i) const override;
    };
}

#endif