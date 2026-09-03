#include "FunctionParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode FunctionParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>&, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    if (!tokens.At(i).CheckType((size_t)TokenType::Identifier) || tokens.At(i).GetValue() != "func") return next();
    i++;
    const MathLib::ParserNode type = next();
    const MathLib::Token name = tokens.At(i++);
    if (!name.CheckType((size_t)TokenType::Identifier)) return MathLib::ParserNode();
    const MathLib::ParserNode args = next();
    const MathLib::ParserNode body = next();
    return MathLib::ParserNode((size_t)TokenType::Function, name.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(
        type, args, body
    ));
}