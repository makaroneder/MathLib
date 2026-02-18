#ifndef MathLib_EquationSolver_BuiltinFunction_H
#define MathLib_EquationSolver_BuiltinFunction_H
#include "Node.hpp"
#include "../FunctionPointer.hpp"

namespace MathLib {
    using BuiltinFunctionPointer = FunctionPointer<Node*, const Sequence<const Node*>&>;
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
        BuiltinFunction(const Sequence<char>& n, const BuiltinFunctionPointer& func);
        [[nodiscard]] bool operator==(const BuiltinFunction& other) const;
        [[nodiscard]] bool operator!=(const BuiltinFunction& other) const;
    };
    /// @brief Creates default builtin functions for equation solver
    /// @return Default builtin functions
    [[nodiscard]] Array<BuiltinFunction> CreateDefaultBuiltinFunctions(void);
}

#endif