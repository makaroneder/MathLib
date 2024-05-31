#ifndef State_H
#define State_H
#include "BuiltinFunction.hpp"
#include "Function.hpp"
#include "Variable.hpp"

struct State {
    Array<BuiltinFunction> builtinFunctions;
    Array<Function> functions;
    Array<Variable> variables;

    State(Array<BuiltinFunction> builtinFuncs = Array<BuiltinFunction>(), Array<Function> funcs = Array<Function>(), Array<Variable> vars = Array<Variable>());
    Function GetFunction(String name) const;
};

#endif