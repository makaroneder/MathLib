#ifndef Parser_H
#define Parser_H
#include "ParserLayer.hpp"

struct Parser : MathLib::Allocatable {
    Parser(const MathLib::Sequence<ParserLayer*>& layers);
    virtual ~Parser(void) override;
    void LoadInput(const MathLib::Sequence<Token>& input);
    [[nodiscard]] Node Parse(void);

    private:
    [[nodiscard]] Node Parse(size_t layer);

    MathLib::Array<ParserLayer*> layers;
    MathLib::Array<Token> tokens;
    size_t i;
};

#endif