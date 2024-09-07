#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>

Question::Question(const String& eq) : equation(eq), solution(MakeNaN()) {
    Node* root = Tokenize(equation);
    Optimizer optimizer = Optimizer();
    Node* optimizedRoot = optimizer.Optimize(root);
    delete root;
    delete optimizedRoot;
    for (Variable& variable : optimizer.variables) {
        if (variable.name == "a") {
            solution = variable.value->ToNumber().At(0).GetReal();
            break;
        }
    }
    optimizer.Destroy();
}