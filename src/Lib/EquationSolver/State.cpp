#include "State.hpp"

State::State(std::vector<BuiltinFunction> builtinFuncs, std::vector<Function> funcs, std::vector<Variable> vars) : builtinFunctions(std::vector<BuiltinFunction>(defaultBuiltinFuncs, defaultBuiltinFuncs + SizeOfArray(defaultBuiltinFuncs))), functions(funcs) {
    builtinFunctions.reserve(builtinFuncs.size());
    for (const BuiltinFunction& func : builtinFuncs) builtinFunctions.push_back(func);
    for (const Variable& var : defaultVariables) variables.push_back(Variable(var.name, var.value->Recreate()));
    for (const Variable& var : vars) variables.push_back(var);
}
const Function* State::GetFunction(std::string name) const {
    for (const Function& func : functions)
        if (func.name == name) return &func;
    return nullptr;
}