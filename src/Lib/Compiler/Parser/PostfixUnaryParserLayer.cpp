#include "PostfixUnaryParserLayer.hpp"

namespace MathLib {
    PostfixUnaryParserLayer::PostfixUnaryParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {}
    ParserNode PostfixUnaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const ParserNode ret = next();
        if (i >= tokens.GetSize()) return ret;
        const Token token = tokens.AtUnsafe(i);
        if (!token.CheckType(tokenType)) return ret;
        i++;
        return ParserNode(nodeType, token.GetValue(), MakeArray<ParserNode>(ret));
    }
}