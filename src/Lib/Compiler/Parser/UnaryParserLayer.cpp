#include "UnaryParserLayer.hpp"

namespace MathLib {
    UnaryParserLayer::UnaryParserLayer(size_t nodeType, size_t tokenType) : nodeType(nodeType), tokenType(tokenType) {
        EmptyBenchmark
    }
    ParserNode UnaryParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        const MathLib::Token token = tokens.At(i);
        if (!token.CheckType(tokenType)) ReturnFromBenchmark(next());
        i++;
        ReturnFromBenchmark(MathLib::ParserNode(nodeType, token.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(next())));
    }
}