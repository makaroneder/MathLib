#ifndef MathLib_Math_Factorial_H
#define MathLib_Math_Factorial_H
#include "../Host.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Gamma(const T& z) {
        T ret = (T)1 / z;
        for (size_t n = 1; true; n++) {
            const T tmp = Pow((T)1 / (T)n + 1, z) / (z / (T)n + 1);
            ret *= tmp;
            if (FloatsEqual<T>(tmp, 1, eps * eps)) break;
        }
        return ret;
    }
    template <typename T>
    [[nodiscard]] T Factorial(const T& z, const T& a) {
        return Pow(a, (z - 1) / a) * Gamma<T>(z / a + 1) / Gamma<T>((T)1 / a + 1);
    }
}

#endif