#ifndef Optimizer_H
#define Optimizer_H
#include <Compiler/Evaluator.hpp>

struct Optimizer : MathLib::Evaluator<MathLib::ParserNode> {
    [[nodiscard]] virtual MathLib::ParserNode Evaluate(const MathLib::ParserNode& node) override;
};

#endif