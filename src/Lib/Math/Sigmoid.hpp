#ifndef MathLib_Math_Sigmoid_H
#define MathLib_Math_Sigmoid_H
#include "../Host.hpp"

namespace MathLib {
    /// @brief 1 / (1 + e^-x)
    /// @tparam T Type of number
    /// @param x Sigmoid input
    /// @return Return value
    template <typename T>
    constexpr T Sigmoid(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one / (Exp(-x) + one));
    }
}

#endif