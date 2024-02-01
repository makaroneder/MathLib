#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include <functional>

/// @brief Builtin function
struct BuiltinFunction {
    /// @brief Name of the function
    std::string name;
    /// @brief Code of the function
    std::function<Node*(std::vector<Node*>)> function;

    /// @brief Creates new function
    /// @param n Name of the function
    /// @param func Code of the function
    BuiltinFunction(std::string n, std::function<Node*(std::vector<Node*>)> func);
};

#endif