#ifndef MathLib_Math_LinearInterpolation_H
#define MathLib_Math_LinearInterpolation_H

namespace MathLib {
    template <typename F, typename T>
    [[nodiscard]] F Lerp(const F& x0, const F& x1, const T& t) {
        return x0 + (x1 - x0) * t;
    }
}

#endif