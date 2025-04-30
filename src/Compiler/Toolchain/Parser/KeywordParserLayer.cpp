#include "KeywordParserLayer.hpp"

KeywordParserLayer::KeywordParserLayer(size_t type, const Token& keyword) : keyword(keyword), type(type) {}
Node KeywordParserLayer::Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const {
    const Token token = tokens.At(i);
    if (token != keyword) return next();
    i++;
    return Node(type, keyword.GetValue(), MathLib::MakeArray<Node>(root()));
}