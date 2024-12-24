#ifndef MathLib_EquationSolver_Tokenizer_H
#define MathLib_EquationSolver_Tokenizer_H
#include "Node.hpp"

namespace MathLib {
    /// @brief Skips whitespace characters
    /// @param str String to skip
    /// @param i Current position in the string
    void SkipWhiteSpace(const String& str, size_t& i);
    /// @brief Tokenizes string into nodes
    /// @param str String to tokenize
    /// @return Tokenized string
    Node* Tokenize(const String& str);
}

#endif