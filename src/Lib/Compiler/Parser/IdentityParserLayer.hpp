#ifndef MathLib_Compiler_Parser_IdentityLayer_H
#define MathLib_Compiler_Parser_IdentityLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct IdentityParserLayer : ParserLayer {
        IdentityParserLayer(size_t nodeType, size_t tokenType);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        size_t nodeType;
        size_t tokenType;
    };
}

#endif