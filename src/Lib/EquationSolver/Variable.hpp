#ifndef MathLib_EquationSolver_Variable_H
#define MathLib_EquationSolver_Variable_H
#include "Node.hpp"

namespace MathLib {
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
        Variable(const Sequence<char>& name, const Sequence<char>& dataType, Node* value, bool constant);
        /// @brief Creates a new variable
        /// @param name Name of the variable
        /// @param dataType Data type of the variable
        /// @param value Value of the variable
        /// @brief Is variable constant?
        Variable(const Sequence<char>& name, const Sequence<char>& dataType, const Sequence<char>& value, bool constant);
        [[nodiscard]] bool operator==(const Variable& other) const;
        [[nodiscard]] bool operator!=(const Variable& other) const;
    };
    /// @brief Creates default variables for equation solver
    /// @return Default variables
    [[nodiscard]] Array<Variable> CreateDefaultVariables(void);
}

#endif