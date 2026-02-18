#ifndef Question_H
#define Question_H
#include <Interfaces/Printable.hpp>

/// @brief Question for our quiz
struct Question : MathLib::Printable {
    /// @brief Creates new question
    /// @param eq Equation to solve
    Question(const MathLib::Sequence<char>& equation);
    [[nodiscard]] bool IsSolution(MathLib::num_t sol, MathLib::num_t eps) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    /// @brief Equation to solve
    MathLib::String equation;
    /// @brief Solution for equation
    MathLib::num_t solution;
};

#endif