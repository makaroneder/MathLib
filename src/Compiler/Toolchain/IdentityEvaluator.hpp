#ifndef IdentityEvaluator_H
#define IdentityEvaluator_H
#include "Evaluator.hpp"

struct IdentityEvaluator : Evaluator<Node> {
    [[nodiscard]] virtual Node Evaluate(const Node& node) override;
};

#endif