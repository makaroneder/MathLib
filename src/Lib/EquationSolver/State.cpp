#include "State.hpp"

EquationSolverState::EquationSolverState(const Array<BuiltinFunction>& builtinFuncs, const Array<FunctionNode>& funcs, const Array<Variable>& vars) : builtinFunctions(CreateDefaultBuiltinFunctions()), functions(funcs), variables(CreateDefaultVariables()), runtime(false) {
    for (const BuiltinFunction& func : builtinFuncs) builtinFunctions.Add(func);
    for (const Variable& var : vars) variables.Add(var);
}
void EquationSolverState::Destroy(void) {
    for (FunctionNode& function : functions) {
        for (Variable& arg : function.arguments) delete arg.value;
        delete function.body;
    }
    for (Variable& variable : variables) delete variable.value;
    builtinFunctions = {};
    functions = {};
    variables = {};
    runtime = false;
}
FunctionNode EquationSolverState::GetFunction(const String& name) const {
    for (const FunctionNode& function : functions)
        if (function.name == name) return function;
    return FunctionNode("", Array<Variable>(), nullptr, "");
}