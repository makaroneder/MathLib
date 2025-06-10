#ifndef MathLib_Compiler_Parser_Layer_H
#define MathLib_Compiler_Parser_Layer_H
#include "ParserNode.hpp"
#include "../Lexer/Token.hpp"

namespace MathLib {
    struct ParserLayer : Allocatable {
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const = 0;
    };
}

#endif