#ifndef Function_H
#define Function_H
#include "Node.hpp"
#include "Variable.hpp"
#include <vector>

/// @brief Function
struct Function {
    /// @brief Name of the function
    std::string name;
    /// @brief Arguments of the function
    std::vector<Variable> arguments;
    /// @brief Code of the function
    Node* body;

    /// @brief Creates new function
    /// @param n Name of the function
    /// @param args Arguments of the function
    /// @param b Code of the function
    Function(std::string n, std::vector<Variable> args, Node* b);
};

#endif