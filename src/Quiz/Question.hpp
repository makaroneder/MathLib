#ifndef Question_H
#define Question_H
#include <Typedefs.hpp>

/// @brief Question for our quiz
struct Question : Allocatable {
    /// @brief Equation to solve
    String equation;
    /// @brief Solution for equation
    num_t solution;

    /// @brief Creates new question
    /// @param eq Equation to solve
    Question(const String& eq);
};

#endif