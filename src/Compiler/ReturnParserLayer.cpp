#include "ReturnParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode ReturnParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>&, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    if (!tokens.At(i).CheckType((size_t)TokenType::Identifier) || tokens.At(i).GetValue() != "return") return next();
    i++;
    return MathLib::ParserNode((size_t)TokenType::Return, ""_M, MathLib::MakeArray<MathLib::ParserNode>(next()));
}