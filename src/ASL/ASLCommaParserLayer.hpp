#ifndef ASLCommaParserLayer_H
#define ASLCommaParserLayer_H
#include <Compiler/Parser/ParserLayer.hpp>

struct ASLCommaParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override;
};

#endif