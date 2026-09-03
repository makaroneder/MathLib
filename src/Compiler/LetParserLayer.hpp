#ifndef LetParserLayer_H
#define LetParserLayer_H
#include <Compiler/Parser/ParserLayer.hpp>

struct LetParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override;
};

#endif