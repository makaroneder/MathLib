#ifndef EquationSolverState_H
#define EquationSolverState_H
#include "BuiltinFunction.hpp"
#include "FunctionNode.hpp"

struct EquationSolverState : Allocatable {
    Array<BuiltinFunction> builtinFunctions;
    Array<FunctionNode> functions;
    Array<Variable> variables;
    bool runtime;

    EquationSolverState(const Array<BuiltinFunction>& builtinFuncs = Array<BuiltinFunction>(), const Array<FunctionNode>& funcs = Array<FunctionNode>(), const Array<Variable>& vars = Array<Variable>());
    void Destroy(void);
    FunctionNode GetFunction(const String& name) const;
};

#endif