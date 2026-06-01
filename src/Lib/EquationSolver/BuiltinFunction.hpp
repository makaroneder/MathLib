#ifndef MathLib_EquationSolver_BuiltinFunction_H
#define MathLib_EquationSolver_BuiltinFunction_H
#include "Node.hpp"
#include "../FunctionPointer.hpp"

namespace MathLib {
    using BuiltinFunctionPointer = FunctionPointer<Node*, const Sequence<const Node*>&>;
    struct BuiltinFunction : Allocatable {
        String name;
        BuiltinFunctionPointer function;

        BuiltinFunction(void);
        BuiltinFunction(const Sequence<char>& n, const BuiltinFunctionPointer& func);
        [[nodiscard]] bool operator==(const BuiltinFunction& other) const;
        [[nodiscard]] bool operator!=(const BuiltinFunction& other) const;
    };
    [[nodiscard]] Array<BuiltinFunction> CreateDefaultBuiltinFunctions(void);
}

#endif