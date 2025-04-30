#ifndef ParserLayer_H
#define ParserLayer_H
#include "Node.hpp"
#include "../Lexer/Token.hpp"

struct ParserLayer : MathLib::Allocatable {
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const = 0;
};

#endif