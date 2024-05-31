#ifndef Combination_H
#define Combination_H
#include "Factorial.hpp"

/// @brief C(n, k) = n! / (k! * (n - k)!)
/// @tparam T Type of number
/// @param n Number of elements
/// @param k Number of elements to select
/// @return Number of ways to select k elements from n elements set
template <typename T>
T Combination(T n, T k) {
    return Factorial<T>(n) / (Factorial<T>(k) * Factorial<T>(n - k));
}

#endif