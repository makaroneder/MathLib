#ifndef MathLib_Compiler_Lexer_Rule_H
#define MathLib_Compiler_Lexer_Rule_H
#include "../../Interfaces/Sequence.hpp"

namespace MathLib {
    struct LexerRule : Allocatable {
        LexerRule(size_t type);
        [[nodiscard]] size_t GetType(void) const;
        virtual void Match(const Sequence<char>& str, size_t& i) const = 0;

        private:
        size_t type;
    };
}

#endif