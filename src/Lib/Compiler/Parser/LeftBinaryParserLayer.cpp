#include "LeftBinaryParserLayer.hpp"

namespace MathLib {
    LeftBinaryParserLayer::LeftBinaryParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
    ParserNode LeftBinaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const size_t size = tokens.GetSize();
        ParserNode ret = next();
        while (i < size) {
            const Token token = tokens.At(i);
            if (!token.CheckType(tokenType)) break;
            i++;
            ret = ParserNode(nodeType, token.GetValue(), MakeArray<ParserNode>(ret, next()));
        }
        return ret;
    }
}