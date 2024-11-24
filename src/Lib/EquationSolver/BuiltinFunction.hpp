#ifndef EquationSolver_BuiltinFunction_H
#define EquationSolver_BuiltinFunction_H
#include "Node.hpp"
#include "../FunctionPointer.hpp"

namespace MathLib {
    using BuiltinFunctionPointer = FunctionPointer<Node*, const Array<const Node*>&>;
    /// @brief Builtin function
    struct BuiltinFunction : Allocatable {
        /// @brief Name of the builtin function
        String name;
        /// @brief Code of the builtin function
        BuiltinFunctionPointer function;

        /// @brief Creates empty builtin function
        BuiltinFunction(void);
        /// @brief Creates new builtin function
        /// @param n Name of the builtin function
        /// @param func Code of the builtin function
        BuiltinFunction(const String& n, const BuiltinFunctionPointer& func);
    };
    /// @brief Creates default builtin functions for equation solver
    /// @return Default builtin functions
    Array<BuiltinFunction> CreateDefaultBuiltinFunctions(void);
}

#endif