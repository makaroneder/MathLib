#ifndef MiddleParserLayer_H
#define MiddleParserLayer_H
#include "ParserLayer.hpp"

struct MiddleParserLayer : ParserLayer {
    MiddleParserLayer(size_t nodeType, size_t tokenType);
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override;

    private:
    size_t nodeType;
    size_t tokenType;
};

#endif