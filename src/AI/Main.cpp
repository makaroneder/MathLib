#include "Term.hpp"
#include <iostream>

int main(int, char**) {
    try {
        const Term<float> term = Term<float>(TermType::Multiplication,
            Term<float>(TermType::Variable, 1),
            Term<float>(TermType::Variable, 1)
        ).Div(Term<float>(TermType::Variable, 2));
        std::cout << term << std::endl;
        MathLib::Dictionary<float, Term<float>> derivatives;
        if (!term.GetDerivatives(derivatives)) MathLib::Panic("Failed to get derivatives");
        std::cout << derivatives << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}