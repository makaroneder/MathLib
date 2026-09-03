#include "LetParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode LetParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>&, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    const MathLib::Token mode = tokens.At(i);
    if (!mode.CheckType((size_t)TokenType::Identifier) || (mode.GetValue() != "let" && mode.GetValue() != "const")) return next();
    i++;
    const MathLib::ParserNode type = next();
    const MathLib::ParserNode name = next();
    return MathLib::ParserNode((size_t)TokenType::Let, mode.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(
        type, name
    ));
}