#ifndef MathLib_Math_Log_H
#define MathLib_Math_Log_H
#include "../Host.hpp"

namespace MathLib {
    /// @brief log_b x = log x / log b
    /// @tparam T Type of number
    /// @param b Base of the logarithm
    /// @param x Value of the logarithm
    /// @return Logarithm of specified base and number
    template <typename T>
    T Log(const T& b, const T& x) {
        StartBenchmark
        ReturnFromBenchmark(NaturalLog(x) / NaturalLog(b));
    }
}

#endif