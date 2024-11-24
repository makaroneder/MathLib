#ifndef Question_H
#define Question_H
#include <Typedefs.hpp>

/// @brief Question for our quiz
struct Question : MathLib::Allocatable {
    /// @brief Equation to solve
    MathLib::String equation;
    /// @brief Solution for equation
    MathLib::num_t solution;

    /// @brief Creates new question
    /// @param eq Equation to solve
    Question(const MathLib::String& eq);
};

#endif