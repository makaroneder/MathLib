#ifndef ASLFunctionParserLayer_H
#define ASLFunctionParserLayer_H
#include <Compiler/Parser/ParserLayer.hpp>

struct ASLFunctionParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override;
};

#endif