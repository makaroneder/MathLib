#ifndef Optimizer_H
#define Optimizer_H
#include <Compiler/Parser/ParserNode.hpp>

struct Optimizer : MathLib::Function<MathLib::ParserNode, MathLib::ParserNode> {
    [[nodiscard]] virtual MathLib::ParserNode Invoke(MathLib::ParserNode node) const override;
};

#endif