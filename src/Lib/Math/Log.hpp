#ifndef MathLib_Math_Log_H
#define MathLib_Math_Log_H
#include "../Host.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Log(const T& b, const T& x) {
        return NaturalLog(x) / NaturalLog(b);
    }
}

#endif