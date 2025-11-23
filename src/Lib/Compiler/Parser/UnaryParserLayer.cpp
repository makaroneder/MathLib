#include "UnaryParserLayer.hpp"

namespace MathLib {
    UnaryParserLayer::UnaryParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
    ParserNode UnaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const Token token = tokens.At(i);
        if (!token.CheckType(tokenType)) return next();
        i++;
        return ParserNode(nodeType, token.GetValue(), MakeArray<ParserNode>(next()));
    }
}