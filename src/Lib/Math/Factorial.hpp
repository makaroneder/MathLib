#ifndef MathLib_Math_Factorial_H
#define MathLib_Math_Factorial_H
#include "../Host.hpp"

namespace MathLib {
    /// @brief z! = gamma(z + 1)
    /// @tparam T Type of number
    /// @param z Complex number for which we want the gamma
    /// @param end Number of iterations
    /// @return Gamma of the complex number
    template <typename T>
    T Gamma(const T& z, const size_t& end = 2000) {
        StartBenchmark
        T ret = (T)1 / z;
        for (size_t n = 1; n < end; n++)
            ret *= Pow((T)1 / (T)n + 1, z) / (z / (T)n + 1);
        ReturnFromBenchmark(ret);
    }
    /// @brief z!(a) = gamma(z / a + 1) / gamma(1 / a + 1)
    /// @tparam T Type of number
    /// @param n Complex number for which we want the factorial
    /// @param a Level of multifactorial
    /// @return Factorial of the complex number
    template <typename T>
    T Factorial(const T& z, const T& a) {
        StartBenchmark
        ReturnFromBenchmark(Pow(a, (z - 1) / a) * Gamma<T>(z / a + 1) / Gamma<T>((T)1 / a + 1));
    }
}

#endif