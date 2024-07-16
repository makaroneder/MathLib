#ifndef FunctionNode_H
#define FunctionNode_H
#include "Variable.hpp"

/// @brief Function
struct FunctionNode : Allocatable {
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
    FunctionNode(void);
    /// @brief Creates new function
    /// @param n Name of the function
    /// @param args Arguments of the function
    /// @param b Code of the function
    FunctionNode(const String& n, const Array<Variable>& args, Node* b, const String& d, const String& c);
};

#endif