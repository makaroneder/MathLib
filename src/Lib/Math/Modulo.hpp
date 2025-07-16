#ifndef MathLib_Math_Modulo_H
#define MathLib_Math_Modulo_H
#include "../Benchmark.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Modulo(const T& x, const T& modulo) {
        StartBenchmark
        T ret = x;
        while (ret >= modulo) ret -= modulo;
        ReturnFromBenchmark(ret);
    }
}

#endif