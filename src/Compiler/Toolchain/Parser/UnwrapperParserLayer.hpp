#ifndef UnwrapperParserLayer_H
#define UnwrapperParserLayer_H
#include "ParserLayer.hpp"

struct UnwrapperParserLayer : ParserLayer {
    UnwrapperParserLayer(size_t start, size_t end);
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override;

    private:
    size_t start;
    size_t end;
};

#endif