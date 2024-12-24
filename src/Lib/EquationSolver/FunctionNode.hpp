#ifndef MathLib_EquationSolver_FunctionNode_H
#define MathLib_EquationSolver_FunctionNode_H
#include "Variable.hpp"

namespace MathLib {
    /// @brief Function
    struct FunctionNode : Allocatable {
        /// @brief Name of the function
        String name;
        /// @brief Arguments of the function
        Array<Variable> arguments;
        /// @brief Code of the function
        Node* body;
        /// @brief Data type of the function
        String dataType;

        /// @brief Creates empty function
        FunctionNode(void);
        /// @brief Creates new function
        /// @param name Name of the function
        /// @param args Arguments of the function
        /// @param body Code of the function
        /// @param dataType Data type of the function
        FunctionNode(const String& name, const Array<Variable>& args, Node* body, const String& dataType);
    };
}

#endif