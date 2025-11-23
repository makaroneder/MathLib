#ifndef MathLib_Math_Modulo_H
#define MathLib_Math_Modulo_H

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Modulo(const T& x, const T& modulo) {
        T ret = x;
        while (ret >= modulo) ret -= modulo;
        return ret;
    }
}

#endif