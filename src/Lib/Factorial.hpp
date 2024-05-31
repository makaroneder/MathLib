#ifndef Factorial_H
#define Factorial_H
#include "Host.hpp"

/// @brief z! = gamma(z + 1)
/// @tparam T Type of number
/// @param z Complex number for which we want the gamma
/// @param end Number of iterations
/// @return Gamma of the complex number
template <typename T>
constexpr std::complex<T> Gamma(std::complex<T> z, T end = 100) {
    const std::complex<T> one = 1;
    std::complex<T> ret = one / z;
    for (T n = 1; n < end; n++)
        ret *= Pow(one + one / n, z) / (one + z / n);
    return ret;
}
/// @brief n! = gamma(n + 1)
/// @tparam T Type of number
/// @param n Number for which we want the factorial
/// @return Factorial of the number
template <typename T>
constexpr T Factorial(T n) {
    return Gamma(n + 1);
}
/// @brief z! = gamma(z + 1)
/// @tparam T Type of number
/// @param n Complex number for which we want the factorial
/// @return Factorial of the complex number
template <typename T>
constexpr std::complex<T> Factorial(std::complex<T> z) {
    const T one = 1;
    return Gamma<T>(z + one);
}

#endif