#ifndef MathLib_MinMax_H
#define MathLib_MinMax_H

namespace MathLib {
    template <typename T>
    [[nodiscard]] constexpr T Max(const T& x, const T& y) {
        return (x < y) ? y : x;
    }
    template <typename T>
    [[nodiscard]] constexpr T Min(const T& x, const T& y) {
        return (x < y) ? x : y;
    }
    template <typename T>
    [[nodiscard]] constexpr T Clamp(const T& x, const T& min, const T& max) {
        if (x < min) return min;
        else if (x > max) return max;
        else return x;
    }
}

#endif