#ifndef MathLib_MinMax_H
#define MathLib_MinMax_H

namespace MathLib {
    template <typename T>
    T Max(const T& x, const T& y) {
        return (x < y) ? y : x;
    }
    template <typename T>
    T Min(const T& x, const T& y) {
        return (x < y) ? x : y;
    }
}

#endif