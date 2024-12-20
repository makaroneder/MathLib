#ifndef Math_Sigmoid_H
#define Math_Sigmoid_H
#include "../Host.hpp"

namespace MathLib {
    /// @brief 1 / (1 + e^-x)
    /// @tparam T Type of number
    /// @param x Sigmoid input
    /// @return Return value
    template <typename T>
    constexpr T Sigmoid(const T& x) {
        const T one = 1;
        return one / (Exp(-x) + one);
    }
}

#endif