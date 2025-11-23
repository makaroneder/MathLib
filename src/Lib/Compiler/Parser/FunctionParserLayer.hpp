#ifndef MathLib_Compiler_Parser_FunctionLayer_H
#define MathLib_Compiler_Parser_FunctionLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct FunctionParserLayer : ParserLayer {
        FunctionParserLayer(size_t nodeType, size_t identifierTokenType, size_t parenthesesTokenType);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        size_t nodeType;
        size_t identifierTokenType;
        size_t parenthesesTokenType;
    };
}

#endif