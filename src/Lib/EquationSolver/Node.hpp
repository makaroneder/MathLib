#ifndef EquationSolver_Node_H
#define EquationSolver_Node_H
#include "../Math/Complex.hpp"

/// @brief Contains mathematical operations, constants, functions and variables
struct Node : Printable {
    /// @brief Type of the node
    enum class Type {
        None,
        Function, Variable, Constant, Array, String,
        Program,
        Comma,
        Equal, DynamicEqual,
        Add, Sub,
        Mul, Div,
        Pow, Root,
        Factorial,
        Absolute,
        Index,
        Integral, Summation, Product,
    } type;
    /// @brief Value of the node
    String value;
    /// @brief Left child of the node
    Node* left;
    /// @brief Right child of the node
    Node* right;

    /// @brief Creates a new node
    /// @param type Node type
    /// @param value Value
    /// @param left Left child
    /// @param right Right child
    Node(Type type, const String& value, Node* left = nullptr, Node* right = nullptr);
    /// @brief Destroys current node and it's children
    virtual ~Node(void) override;
    /// @brief Creates a new node with the same values as the current node
    /// @return New node
    Node* Recreate(void) const;
    /// @brief Converts node to number array
    /// @return Number array
    Array<complex_t> ToNumber(void) const;
    /// @brief Converts values of the current node to string
    /// @param padding Padding of the generated strings
    virtual String ToString(const String& padding = "") const override;
};
/// @brief Converts node to array
/// @param node Node to convert
/// @return Converted array
Array<const Node*> CommaToArray(const Node* node);
/// @brief Converts array to node
/// @param array Array to convert
/// @return Converted node
Node* ArrayToComma(const Array<Node*>& array);
/// @brief Replaces every node containing specified variable with specified replacement
/// @param node Root node to replace
/// @param name Name of the variable to replace
/// @param replacement Node to replace with
/// @return New root node
Node* ReplaceVariable(const Node* node, const String& name, const Node* replacement);
/// @brief Checks whether node contains specified variable
/// @param node Root node to check
/// @param name Name of the variable
/// @return Status
bool ContainsVariable(const Node* node, const String& name);

#endif