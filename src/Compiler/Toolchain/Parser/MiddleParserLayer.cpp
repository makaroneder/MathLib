#include "MiddleParserLayer.hpp"

MiddleParserLayer::MiddleParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
Node MiddleParserLayer::Parse(const MathLib::Function<Node>&, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const {
    Node ret = next();
    const size_t size = tokens.GetSize();
    while (i < size) {
        const Token token = tokens.At(i);
        if (!token.CheckType(tokenType)) break;
        i++;
        ret = Node(nodeType, token.GetValue(), MathLib::MakeArray<Node>(ret, next()));
    }
    return ret;
}