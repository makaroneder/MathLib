#ifndef MathLib_EquationSolver_Variable_H
#define MathLib_EquationSolver_Variable_H
#include "Node.hpp"

namespace MathLib {
    struct Variable : Allocatable {
        String name;
        String dataType;
        Node* value;
        bool constant;

        Variable(void);
        Variable(const Sequence<char>& name, const Sequence<char>& dataType, Node* value, bool constant);
        Variable(const Sequence<char>& name, const Sequence<char>& dataType, const Sequence<char>& value, bool constant);
        [[nodiscard]] bool operator==(const Variable& other) const;
        [[nodiscard]] bool operator!=(const Variable& other) const;
    };
    [[nodiscard]] Array<Variable> CreateDefaultVariables(void);
}

#endif