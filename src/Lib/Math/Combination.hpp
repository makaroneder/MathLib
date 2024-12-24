#ifndef MathLib_Math_Combination_H
#define MathLib_Math_Combination_H
#include "Factorial.hpp"

namespace MathLib {
    /// @brief C(n, k) = n! / (k! * (n - k)!)
    /// @tparam T Type of number
    /// @param n Number of elements
    /// @param k Number of elements to select
    /// @return Number of ways to select k elements from n elements set
    template <typename T>
    T Combination(const T& n, const T& k) {
        StartBenchmark
        ReturnFromBenchmark(Factorial<T>(n, 1) / (Factorial<T>(k, 1) * Factorial<T>(n - k, 1)));
    }
}

#endif