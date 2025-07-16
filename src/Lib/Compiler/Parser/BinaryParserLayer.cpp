#include "BinaryParserLayer.hpp"

namespace MathLib {
    BinaryParserLayer::BinaryParserLayer(size_t nodeType, size_t tokenType, bool left) : nodeType(nodeType), tokenType(tokenType), left(left) {
        EmptyBenchmark
    }
    ParserNode BinaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        ParserNode ret = next();
        const size_t size = tokens.GetSize();
        while (i < size) {
            const Token token = tokens.At(i);
            if (!token.CheckType(tokenType)) break;
            i++;
            const ParserNode tmp = next();
            ret = ParserNode(nodeType, token.GetValue(), left ? MakeArray<ParserNode>(ret, tmp) : MakeArray<ParserNode>(tmp, ret));
        }
        ReturnFromBenchmark(ret);
    }
}