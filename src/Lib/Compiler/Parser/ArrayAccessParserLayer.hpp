#ifndef MathLib_Compiler_Parser_ArrayAccessLayer_H
#define MathLib_Compiler_Parser_ArrayAccessLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct ArrayAccessParserLayer : ParserLayer {
        ArrayAccessParserLayer(size_t type, size_t start, size_t end);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        size_t type;
        size_t start;
        size_t end;
    };
}

#endif