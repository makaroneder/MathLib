#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>

Question::Question(std::string eq) {
    equation = eq;
    State state;
    state.variables = std::vector<Variable>(defaultVariables, defaultVariables + SizeOfArray(defaultVariables));
    state.builtinFunctions = std::vector<BuiltinFunction>(defaultBuiltinFuncs, defaultBuiltinFuncs + SizeOfArray(defaultBuiltinFuncs));
    Node* root = Tokenize(equation);
    Node* optimizedRoot = Optimize(root, state);
    delete root;
    for (const Variable& var : state.variables) {
        if (var.name == "a") {
            solution = std::stold(var.value);
            break;
        }
    }
    delete optimizedRoot;
}