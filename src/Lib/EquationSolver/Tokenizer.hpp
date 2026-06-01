#ifndef MathLib_EquationSolver_Tokenizer_H
#define MathLib_EquationSolver_Tokenizer_H
#include "Node.hpp"

namespace MathLib {
    [[nodiscard]] Node* Tokenize(const Sequence<char>& str);
}

#endif