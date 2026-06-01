#ifndef MathLib_Math_Sigmoid_H
#define MathLib_Math_Sigmoid_H
#include "../Host.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Sigmoid(const T& x) {
        return T(1) / (Exp(-x) + 1);
    }
}

#endif