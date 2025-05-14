#ifndef Optimizer_H
#define Optimizer_H
#include "Toolchain/Evaluator.hpp"

struct Optimizer : Evaluator<Node> {
    [[nodiscard]] virtual Node Evaluate(const Node& node) override;
};

#endif