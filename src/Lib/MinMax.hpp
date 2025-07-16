#ifndef MathLib_MinMax_H
#define MathLib_MinMax_H
#include "Benchmark.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] constexpr T Max(const T& x, const T& y) {
        StartAndReturnFromBenchmark((x < y) ? y : x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Min(const T& x, const T& y) {
        StartAndReturnFromBenchmark((x < y) ? x : y);
    }
    template <typename T>
    [[nodiscard]] constexpr T Clamp(const T& x, const T& min, const T& max) {
        StartBenchmark
        if (x < min) ReturnFromBenchmark(min);
        if (x > max) ReturnFromBenchmark(max);
        ReturnFromBenchmark(x);
    }
}

#endif