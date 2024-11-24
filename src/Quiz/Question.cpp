#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Host.hpp>

Question::Question(const MathLib::String& eq) : equation(eq), solution(MathLib::MakeNaN()) {
    MathLib::Node* root = MathLib::Tokenize(equation);
    MathLib::Optimizer optimizer = MathLib::Optimizer();
    MathLib::Node* optimizedRoot = optimizer.Optimize(root);
    delete root;
    delete optimizedRoot;
    for (MathLib::Variable& variable : optimizer.variables) {
        if (variable.name == "a") {
            solution = variable.value->ToNumber().At(0).GetReal();
            break;
        }
    }
    optimizer.Destroy();
}