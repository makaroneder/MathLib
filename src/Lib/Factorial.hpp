#ifndef Factorial_H
#define Factorial_H
#include "Product.hpp"
#include "Typedefs.hpp"
#include <cmath>

/// @brief Limit of iterations for function Gamma
static constexpr num_t gammaEnd = 100;
/// @brief f(z) = (1 / z) * (1 + 1 / 1)^z / (1 + z / 1) * ... * (1 + 1 / ∞)^z / (1 + z / ∞)
/// @tparam T Type of number
/// @param z Number which we want this function to calculate (if it's negative than it can't be integer)
/// @return Value of function
template <typename T>
T Gamma(T z) {
    return (1 / z) * Product<T>(1, gammaEnd, [z](T n) {
        return std::pow<T, T>(1 + 1 / n, z) / (1 + z / n);
    });
}
/// @brief 0! = 1 and n! = n * (n - 1)!
/// @tparam T Type of number
/// @param n Number for which we want the factorial
/// @return Factorial of the number
template <typename T>
T Factorial(T n) {
    return Gamma<T>(n + 1);
}

#endif