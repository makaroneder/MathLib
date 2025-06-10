#include "KeywordParserLayer.hpp"

namespace MathLib {
    KeywordParserLayer::KeywordParserLayer(size_t type, const Token& keyword) : keyword(keyword), type(type) {
        EmptyBenchmark
    }
    ParserNode KeywordParserLayer::Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        StartBenchmark
        const Token token = tokens.At(i);
        if (token != keyword) ReturnFromBenchmark(next());
        i++;
        ReturnFromBenchmark(ParserNode(type, keyword.GetValue(), MakeArray<ParserNode>(root())));
    }
}