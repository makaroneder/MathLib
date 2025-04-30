#ifndef Evaluator_H
#define Evaluator_H
#include "Parser/Node.hpp"

template <typename T>
struct Evaluator : MathLib::Allocatable {
    [[nodiscard]] virtual T Evaluate(const Node& node) = 0;
};

#endif