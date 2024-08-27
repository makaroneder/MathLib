#ifndef Variable_H
#define Variable_H
#include "../Constants.hpp"
#include "Node.hpp"

/// @brief Contains the name, value and type of the variable
struct Variable : Allocatable {
    /// @brief Name of the variable
    String name;
    /// @brief Data type of the variable
    String dataType;
    /// @brief Value of the variable
    Node* value;
    /// @brief Is variable constant?
    bool constant;

    /// @brief Creates empty variable
    Variable(void);
    /// @brief Creates a new variable
    /// @param name Name of the variable
    /// @param dataType Data type of the variable
    /// @param value Value of the variable
    /// @brief Is variable constant?
    Variable(const String& name, const String& dataType, Node* value, bool constant);
    /// @brief Creates a new variable
    /// @param name Name of the variable
    /// @param dataType Data type of the variable
    /// @param value Value of the variable
    /// @brief Is variable constant?
    Variable(const String& name, const String& dataType, const String& value, bool constant);
};
/// @brief Creates default variables for equation solver
/// @return Default variables
Array<Variable> CreateDefaultVariables(void);

#endif