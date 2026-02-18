#include "RightBinaryParserLayer.hpp"

namespace MathLib {
    RightBinaryParserLayer::RightBinaryParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
    ParserNode RightBinaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const size_t size = tokens.GetSize();
        ParserNode ret = next();
        ParserNode* curr = &ret;
        while (i < size) {
            const Token token = tokens.AtUnsafe(i);
            if (!token.CheckType(tokenType)) break;
            i++;
            *curr = ParserNode(nodeType, token.GetValue(), MakeArray<ParserNode>(*curr, next()));
            curr = &curr->AtUnsafe(1);
        }
        return ret;
    }
}