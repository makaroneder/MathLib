#ifndef MathLib_Compiler_Parser_LeftBinaryLayer_H
#define MathLib_Compiler_Parser_LeftBinaryLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct LeftBinaryParserLayer : ParserLayer {
        LeftBinaryParserLayer(size_t nodeType, size_t tokenType);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        size_t nodeType;
        size_t tokenType;
    };
}

#endif