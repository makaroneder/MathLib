#include "RewrapperParserLayer.hpp"

namespace MathLib {
    RewrapperParserLayer::RewrapperParserLayer(const ParserNode& base, size_t start, size_t end) : base(base), start(start), end(end) {}
    ParserNode RewrapperParserLayer::Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const size_t save = i;
        if (!tokens.At(i).CheckType(start)) return next();
        i++;
        ParserNode ret = base;
        if (!ret.Add(root())) return ParserNode();
        if (!tokens.At(i).CheckType(end)) {
            i = save;
            return next();
        }
        i++;
        return ret;
    }
}