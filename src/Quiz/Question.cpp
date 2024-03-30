#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>

Question::Question(std::string eq) {
    equation = eq;
    solution = NAN;
    Node* root = Tokenize(equation);
    State state;
    Node* optimizedRoot = Optimize(root, state);
    delete root;
    delete optimizedRoot;
    for (const Variable& var : state.variables) {
        if (var.name == "a") {
            solution = std::stold(var.value->ToString());
            break;
        }
    }
}