#ifndef Fibbonacci_H
#define Fibbonacci_H
#include <cmath>

/// @brief (sqrt(5) * ((1 + sqrt(5)) / 2)^n - ((1 - sqrt(5)) / 2)^n) / 5
/// @tparam T Type of number
/// @param n Number in sequence to calculate
/// @return Value of requested number in sequence
template <typename T>
T Fibbonacci(T n) {
    return (std::sqrt<T>(5) * (std::pow<T>((1 + std::sqrt<T>(5)) / 2, n) - std::pow<T>((1 - std::sqrt<T>(5)) / 2, n))) / 5;
}

#endif