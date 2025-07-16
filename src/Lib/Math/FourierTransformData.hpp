#ifndef MathLib_Math_FourierTransformData_H
#define MathLib_Math_FourierTransformData_H
#include "Complex.hpp"

namespace MathLib {
    template <typename T>
    struct FourierTransformData {
        const Function<Complex<T>, T>* func;
        T x;
        bool inverse;

        FourierTransformData(const Function<Complex<T>, T>* func, const T& x, bool inverse) : func(func), x(x), inverse(inverse) {
            EmptyBenchmark
        }
    };
}

#endif