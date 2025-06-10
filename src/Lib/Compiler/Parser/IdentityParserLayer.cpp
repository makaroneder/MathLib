#include "IdentityParserLayer.hpp"

namespace MathLib {
    IdentityParserLayer::IdentityParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {
        EmptyBenchmark
    }
    ParserNode IdentityParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        const Token token = tokens.At(i);
        if (!token.CheckType(tokenType)) ReturnFromBenchmark(next());
        i++;
        ReturnFromBenchmark(ParserNode(nodeType, token.GetValue(), Array<ParserNode>()));
    }
}