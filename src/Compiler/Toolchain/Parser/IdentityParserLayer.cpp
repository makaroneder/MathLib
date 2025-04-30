#include "IdentityParserLayer.hpp"

IdentityParserLayer::IdentityParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
Node IdentityParserLayer::Parse(const MathLib::Function<Node>&, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const {
    const Token token = tokens.At(i);
    if (!token.CheckType(tokenType)) return next();
    i++;
    return Node(nodeType, token.GetValue(), MathLib::Array<Node>());
}