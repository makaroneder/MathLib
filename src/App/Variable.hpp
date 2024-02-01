#ifndef Variable_H
#define Variable_H
#include <string>

/// @brief Contains the name, value and type of the variable
struct Variable {
    /// @brief Name of the variable
    std::string name;
    /// @brief Value of the variable
    std::string value;

    /// @brief Creates a new variable
    /// @param n Name of the variable
    /// @param val Value of the variable
    Variable(std::string n, std::string val);
};

#endif