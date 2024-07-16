#include "State.hpp"

EquationSolverState::EquationSolverState(const Array<BuiltinFunction>& builtinFuncs, const Array<FunctionNode>& funcs, const Array<Variable>& vars) : builtinFunctions(builtinFuncs), functions(funcs), variables(CreateDefaultVariables()), runtime(false) {
    for (size_t i = 0; i < SizeOfArray(defaultBuiltinFuncs); i++)
        builtinFunctions.Add(defaultBuiltinFuncs[i]);
    for (size_t i = 0; i < vars.GetSize(); i++) variables.Add(vars.At(i));
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
    return FunctionNode("", Array<Variable>(), nullptr, "", "");
}