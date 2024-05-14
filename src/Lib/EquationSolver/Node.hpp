#ifndef Node_H
#define Node_H
#include "../Typedefs.hpp"
#include "../Printable.hpp"
#include <functional>
#include <string>
#include <vector>

/// @brief Contains mathematical operations, constants, functions and variables
struct Node : Printable {
    /// @brief Type of the node
    enum class Type {
        None,
        Function, Variable, Constant, ComplexConstant, Array, String,
        Comma,
        Equal,
        Add, Sub,
        Mul, Div,
        Pow, Root,
        Factorial,
        Absolute,
        Index,
        Integral, Summation, Product,
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
    virtual ~Node(void) override;
    /// @brief Is the node constant
    /// @return True if the node is constant
    bool IsConstant(void) const;
    /// @brief Creates a new node with the same values as the current node
    /// @return New node
    Node* Recreate(void) const;
    /// @brief Converts node to number array
    /// @return Number array
    std::vector<complex_t> ToNumber(void) const;
    /// @brief Converts values of the current node to string
    /// @param padding Padding of the generated strings
    virtual std::string ToString(std::string padding = "") const override;
};
/// @brief Converts node to array
/// @param node Node to convert
/// @return Converted array
std::vector<const Node*> CommaToArray(const Node* node);
/// @brief Converts array to node
/// @param array Array to convert
/// @return Converted node
Node* ArrayToComma(std::vector<Node*> array);
/// @brief Replaces every node with a new node
/// @param node Root node to replace
/// @param f Function returning new nodes based on old nodes
/// @return New root node
Node* ReplaceNode(const Node* node, std::function<Node*(const Node*)> f);
/// @brief Checks whether node contains other node
/// @param node Root node to check
/// @param f Function returning wheter node contains other node
/// @return Status
bool ContainsNode(const Node* node, std::function<bool(const Node*)> f);

#endif