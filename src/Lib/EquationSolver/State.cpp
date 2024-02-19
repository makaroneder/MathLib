#include "State.hpp"

State::State(std::vector<BuiltinFunction> builtinFuncs, std::vector<Function> funcs, std::vector<Variable> vars) {
    builtinFunctions = std::vector<BuiltinFunction>(defaultBuiltinFuncs, defaultBuiltinFuncs + SizeOfArray(defaultBuiltinFuncs));
    builtinFunctions.reserve(builtinFuncs.size());
    for (const BuiltinFunction& func : builtinFuncs) builtinFunctions.push_back(func);
    functions = funcs;
    variables = std::vector<Variable>(defaultVariables, defaultVariables + SizeOfArray(defaultVariables));
    variables.reserve(vars.size());
    for (const Variable& var : vars) variables.push_back(var);
}