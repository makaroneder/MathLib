#ifndef Factorial_H
#define Factorial_H
#include <cmath>

/// @brief n! = gamma(n + 1)
/// @tparam T Type of number
/// @param n Number for which we want the factorial
/// @return Factorial of the number
template <typename T>
constexpr T Factorial(T n) {
    return std::tgamma(n + 1);
}

#endif