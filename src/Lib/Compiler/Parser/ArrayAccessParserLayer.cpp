#include "ArrayAccessParserLayer.hpp"

namespace MathLib {
    ArrayAccessParserLayer::ArrayAccessParserLayer(size_t type, size_t start, size_t end) : type(type), start(start), end(end) {}
    ParserNode ArrayAccessParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const ParserNode ret = next();
        if (i >= tokens.GetSize() || !tokens.At(i).CheckType(start)) return ret;
        i++;
        const ParserNode index = next();
        if (!tokens.At(i++).CheckType(end)) return ParserNode();
        return ParserNode(type, ""_M, MakeArray<ParserNode>(ret, index));
    }
}