#ifndef Node_H
#define Node_H
#include <string>
#include <vector>

/// @brief Contains mathematical operations, constants, functions and variables
struct Node {
    /// @brief Type of the node
    enum class Type {
        None,
        Function, Variable, Constant,
        Comma,
        Equal,
        Add, Sub,
        Mul, Div,
        Pow, Root,
        Factorial,
        Absolute,
    } type;
    /// @brief Value of the node
    std::string value;
    /// @brief Left child of the node
    Node* left;
    /// @brief Right child of the node
    Node* right;

    /// @brief Creates a new node
    /// @param t Node type
    /// @param val Value
    /// @param l Left child
    /// @param r Right child
    Node(Type t, std::string val, Node* l = nullptr, Node* r = nullptr);
    /// @brief Destroys current node and it's children
    ~Node(void);
    /// @brief Creates a new node with the same values as the current node
    /// @return New node
    Node* Recreate(void);
    /// @brief Converts values of the current node to string
    /// @param padding Padding of the generated strings
    std::string ToString(std::string padding = "");
};
/// @brief Converts node to array
/// @param node Node to convert
/// @return Converted array
std::vector<Node*> CommaToArray(Node* node);

#endif