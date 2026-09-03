#include "ScopeParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode ScopeParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    if (!tokens.At(i).CheckType((size_t)TokenType::BracketsStart)) return next();
    i++;
    MathLib::ParserNode ret = MathLib::ParserNode((size_t)TokenType::Scope);
    while (true) {
        if (tokens.At(i).CheckType((size_t)TokenType::BracketsEnd)) break;
        if (!ret.Add(root()) || !tokens.At(i++).CheckType((size_t)TokenType::Semicolon)) return MathLib::ParserNode();
    }
    i++;
    return ret;
}