#ifndef Node_H
#define Node_H
#include <string>

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
        Log,
        Absolute,
    } type;
    /// @brief String representation of the node type
    static constexpr const char* typeStr[] = {
        "None",
        "Function", "Variable", "Constant",
        ",",
        "=",
        "+", "-",
        "*", "/",
        "^", "$",
        "!",
        "log",
        "| |",
    };
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
    /// @brief Prints values of the current node
    /// @param padding Padding of the strings to print
    void Print(std::string padding = "");
};

#endif