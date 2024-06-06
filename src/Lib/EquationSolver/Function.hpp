#ifndef Function_H
#define Function_H
#include "Node.hpp"
#include "Variable.hpp"

/// @brief Function
struct Function {
    /// @brief Name of the function
    String name;
    /// @brief Arguments of the function
    Array<Variable> arguments;
    /// @brief Code of the function
    Node* body;
    /// @brief Domain of the function
    String domain;
    /// @brief Codomain of the function
    String codomain;

    /// @brief Creates empty function
    Function(void);
    /// @brief Creates new function
    /// @param n Name of the function
    /// @param args Arguments of the function
    /// @param b Code of the function
    Function(String n, Array<Variable> args, Node* b, String d, String c);
};

#endif