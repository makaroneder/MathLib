#ifndef MathLib_Compiler_Parser_RewrapperLayer_H
#define MathLib_Compiler_Parser_RewrapperLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct RewrapperParserLayer : ParserLayer {
        RewrapperParserLayer(const ParserNode& base, size_t start, size_t end);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        ParserNode base;
        size_t start;
        size_t end;
    };
}

#endif