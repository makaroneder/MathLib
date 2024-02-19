#ifndef Area_H
#define Area_H
#include "Summation.hpp"

/// @brief (b - a) / n * (f(a + i * (b - a) / n) + ... + f(a + (n - 1) * (b - a) / n))
/// @tparam T Type of number
/// @param a Starting point
/// @param b Ending point
/// @param n Number of steps used to calculate area
/// @param f Function generating heights of area to calculate
/// @return Area
template <typename T>
T CalculateArea(T a, T b, T n, std::function<T(T)> f) {
    const T step = (b - a) / n;
    return step * Summation<T>(0, n, [a, b, f, step](T i) {
        return f(a + i * step);
    });
}

#endif