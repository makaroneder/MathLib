#ifndef Optimizer_H
#define Optimizer_H
#include "Node.hpp"
#include "Variable.hpp"
#include "Function.hpp"
#include "BuiltinFunction.hpp"
#include <vector>
#include <functional>

/// @brief Converts node to array
/// @param node Node to convert
/// @return Converted array
std::vector<Node*> CommaToArray(Node* node);
/// @brief Optimizes given node based on the given variables and creates new variables
/// @param node Node to optimize
/// @param variables Variable values to use
/// @param builtinFuncs Builtin functions
/// @param funcs Functions to use
/// @return Optimized node and new variables
Node* Optimize(Node* node, std::vector<Variable>& variables, std::vector<Function>& funcs, std::vector<BuiltinFunction> builtinFuncs);

#endif