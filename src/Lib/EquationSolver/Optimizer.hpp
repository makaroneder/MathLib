#ifndef Optimizer_H
#define Optimizer_H
#include "Node.hpp"
#include "State.hpp"

/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param state Current state
/// @return Optimized node and new variables
Node* Optimize(const Node* node, State& state);

#endif