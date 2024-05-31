#ifndef Question_H
#define Question_H
#include <Typedefs.hpp>
#include <string>

/// @brief Question for our quiz
struct Question {
    /// @brief Equation to solve
    String equation;
    /// @brief Solution for equation
    num_t solution;

    /// @brief Creates new question
    /// @param eq Equation to solve
    Question(String eq);
};

#endif