#include "State.hpp"

State::State(Array<BuiltinFunction> builtinFuncs, Array<Function> funcs, Array<Variable> vars) : builtinFunctions(builtinFuncs), functions(funcs), variables(vars) {
    for (size_t i = 0; i < SizeOfArray(defaultBuiltinFuncs); i++)
        builtinFunctions.Add(defaultBuiltinFuncs[i]);
    for (size_t i = 0; i < SizeOfArray(defaultVariables); i++)
        variables.Add(Variable(defaultVariables[i].name, defaultVariables[i].value->Recreate()));
}
Function State::GetFunction(String name) const {
    for (size_t i = 0; i < functions.GetSize(); i++)
        if (functions.At(i).name == name) return functions.At(i);
    return Function("", Array<Variable>(), nullptr, "", "");
}