#include "UnwrapperParserLayer.hpp"

namespace MathLib {
    UnwrapperParserLayer::UnwrapperParserLayer(size_t start, size_t end) : start(start), end(end) {
        EmptyBenchmark
    }
    ParserNode UnwrapperParserLayer::Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        if (!tokens.At(i).CheckType(start)) ReturnFromBenchmark(next());
        i++;
        const ParserNode ret = root();
        if (!tokens.At(i).CheckType(end)) ReturnFromBenchmark(next());
        i++;
        ReturnFromBenchmark(ret);
    }
}