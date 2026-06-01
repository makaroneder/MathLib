#ifndef MathLib_Math_Combination_H
#define MathLib_Math_Combination_H
#include "Factorial.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Combination(const T& n, const T& k) {
        return Factorial<T>(n, 1) / (Factorial<T>(k, 1) * Factorial<T>(n - k, 1));
    }
}

#endif