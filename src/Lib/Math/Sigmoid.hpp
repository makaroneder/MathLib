#ifndef MathLib_Math_Sigmoid_H
#define MathLib_Math_Sigmoid_H
#include "../Host.hpp"

namespace MathLib {
    /// @brief 1 / (1 + e^-x)
    /// @tparam T Type of number
    /// @param x Sigmoid input
    /// @return Return value
    template <typename T>
    [[nodiscard]] T Sigmoid(const T& x) {
        return T(1) / (Exp(-x) + 1);
    }
}

#endif