#include "UnwrapperParserLayer.hpp"

UnwrapperParserLayer::UnwrapperParserLayer(size_t start, size_t end) : start(start), end(end) {}
Node UnwrapperParserLayer::Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const {
    if (!tokens.At(i).CheckType(start)) return next();
    i++;
    const Node ret = root();
    if (!tokens.At(i).CheckType(end)) return next();
    i++;
    return ret;
}