#include "ASLCommaParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode ASLCommaParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>&, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    const size_t size = tokens.GetSize();
    if (tokens.At(i).CheckType((size_t)TokenType::ParenthesesEnd) || tokens.At(i).CheckType((size_t)TokenType::BracketsEnd) || tokens.At(i).CheckType((size_t)TokenType::BracesEnd)) return MathLib::ParserNode();
    MathLib::ParserNode curr = next();
    while (i < size) {
        if (!tokens.At(i).CheckType((size_t)TokenType::Comma)) break;
        i++;
        const MathLib::ParserNode tmp = (tokens.At(i).CheckType((size_t)TokenType::Comma) || tokens.At(i).CheckType((size_t)TokenType::ParenthesesEnd) || tokens.At(i).CheckType((size_t)TokenType::BracketsEnd) || tokens.At(i).CheckType((size_t)TokenType::BracesEnd)) ? MathLib::ParserNode((size_t)TokenType::None) : next();
        curr = MathLib::ParserNode((size_t)TokenType::Comma, ""_M, MathLib::MakeArray<MathLib::ParserNode>(curr, tmp));
    }
    return curr;
}