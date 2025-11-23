#include "FunctionParserLayer.hpp"

namespace MathLib {
    FunctionParserLayer::FunctionParserLayer(size_t nodeType, size_t identifierTokenType, size_t parenthesesTokenType) : nodeType(nodeType), identifierTokenType(identifierTokenType), parenthesesTokenType(parenthesesTokenType) {}
    ParserNode FunctionParserLayer::Parse(const Function<ParserNode>&, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const {
        const Token identifier = tokens.At(i);
        if (!identifier.CheckType(identifierTokenType) || !tokens.At(i + 1).CheckType(parenthesesTokenType)) return next();
        ++i;
        return ParserNode(nodeType, identifier.GetValue(), MakeArray<ParserNode>(next()));
    }
}