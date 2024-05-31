#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>

Question::Question(String eq) : equation(eq), solution(NAN) {
    Node* root = Tokenize(equation);
    State state;
    Node* optimizedRoot = Optimize(root, state);
    delete root;
    delete optimizedRoot;
    for (size_t i = 0; i < state.variables.GetSize(); i++) {
        if (state.variables.At(i).name == "a") {
            solution = StringToNumber(state.variables.At(i).value->ToString());
            break;
        }
    }
}