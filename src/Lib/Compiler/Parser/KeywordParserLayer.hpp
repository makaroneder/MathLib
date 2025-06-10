#ifndef MathLib_Compiler_Parser_KeywordLayer_H
#define MathLib_Compiler_Parser_KeywordLayer_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct KeywordParserLayer : ParserLayer {
        KeywordParserLayer(size_t type, const Token& keyword);
        [[nodiscard]] virtual ParserNode Parse(const Function<ParserNode>& root, const Function<ParserNode>& next, const Sequence<Token>& tokens, size_t& i) const override;

        private:
        Token keyword;
        size_t type;
    };
}

#endif