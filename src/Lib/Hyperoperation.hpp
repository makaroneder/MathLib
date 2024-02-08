#ifndef Hyperoperation_H
#define Hyperoperation_H
#include <stddef.h>

/// @brief H_n(a, b) = a[n]b
/// @tparam T Type of number
/// @param n Level of hyperoperation
/// @param a Input 1
/// @param b Input 2
/// @return Result of hyperoperation
template <typename T>
constexpr T Hyperoperation(size_t n, T a, T b) {
    switch (n) {
        case 0: return b + 1;
        case 1: return a + b;
        case 2: return a * b;
        default: return b == 0 ? 1 : Hyperoperation<T>(n - 1, a, Hyperoperation<T>(n, a, b - 1));
    }
}

#endif