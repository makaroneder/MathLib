#include "MiddleParserLayer.hpp"

namespace MathLib {
    MiddleParserLayer::MiddleParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
    ParserNode MiddleParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        ParserNode ret = next();
        const size_t size = tokens.GetSize();
        while (i < size) {
            const Token token = tokens.At(i);
            if (!token.CheckType(tokenType)) break;
            i++;
            ret = ParserNode(nodeType, token.GetValue(), MakeArray<ParserNode>(ret, next()));
        }
        ReturnFromBenchmark(ret);
    }
}