#include "Question.hpp"
#include <EquationSolver/Tokenizer.hpp>
#include <EquationSolver/Optimizer.hpp>
#include <Host.hpp>

Question::Question(const MathLib::String& equation) : equation(equation), solution(MathLib::nan) {
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
bool Question::IsSolution(MathLib::num_t sol, MathLib::num_t eps) const {
    return MathLib::FloatsEqual<MathLib::num_t>(solution, sol, eps);
}
MathLib::String Question::ToString(const MathLib::String& padding) const {
    return padding + equation;
}