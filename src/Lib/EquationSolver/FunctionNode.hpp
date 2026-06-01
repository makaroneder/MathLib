#ifndef MathLib_EquationSolver_FunctionNode_H
#define MathLib_EquationSolver_FunctionNode_H
#include "Variable.hpp"

namespace MathLib {
    struct FunctionNode : Allocatable {
        String name;
        Array<Variable> arguments;
        Node* body;
        String dataType;

        FunctionNode(void);
        FunctionNode(const Sequence<char>& name, const Sequence<Variable>& arguments, Node* body, const Sequence<char>& dataType);
        [[nodiscard]] bool operator==(const FunctionNode& other) const;
        [[nodiscard]] bool operator!=(const FunctionNode& other) const;
    };
}

#endif