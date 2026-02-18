#ifndef MathLib_EquationSolver_Tokenizer_H
#define MathLib_EquationSolver_Tokenizer_H
#include "Node.hpp"

namespace MathLib {
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @return Tokenized string
    [[nodiscard]] Node* Tokenize(const Sequence<char>& str);
}

#endif