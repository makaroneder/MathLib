#include "KeywordParserLayer.hpp"

namespace MathLib {
    KeywordParserLayer::KeywordParserLayer(size_t type, const Token& keyword) : keyword(keyword), type(type) {}
    ParserNode KeywordParserLayer::Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const Token token = tokens.At(i);
        if (token != keyword) return next();
        i++;
        return ParserNode(type, keyword.GetValue(), MakeArray<ParserNode>(root()));
    }
}