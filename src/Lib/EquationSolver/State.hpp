#ifndef State_H
#define State_H
#include "BuiltinFunction.hpp"
#include "Function.hpp"
#include "Variable.hpp"

struct State {
    std::vector<BuiltinFunction> builtinFunctions;
    std::vector<Function> functions;
    std::vector<Variable> variables;

    State(std::vector<BuiltinFunction> builtinFuncs = {}, std::vector<Function> funcs = {}, std::vector<Variable> vars = {});
    const Function* GetFunction(std::string name) const;
};

#endif