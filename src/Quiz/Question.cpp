#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>

Question::Question(const String& eq) : equation(eq), solution(MakeNaN()) {
    Node* root = Tokenize(equation);
    EquationSolverState state;
    Node* optimizedRoot = Optimize(root, state);
    delete root;
    delete optimizedRoot;
    for (Variable& variable : state.variables) {
        if (variable.name == "a") {
            solution = variable.value->ToNumber().At(0).GetReal();
            break;
        }
    }
    state.Destroy();
}