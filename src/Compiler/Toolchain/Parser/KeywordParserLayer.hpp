#ifndef KeywordParserLayer_H
#define KeywordParserLayer_H
#include "ParserLayer.hpp"

struct KeywordParserLayer : ParserLayer {
    KeywordParserLayer(size_t type, const Token& keyword);
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override;

    private:
    Token keyword;
    size_t type;
};

#endif