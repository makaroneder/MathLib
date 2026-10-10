#ifndef MathLib_CPU_Sqrt_H
#define MathLib_CPU_Sqrt_H
#include "../Typedefs.hpp"

namespace MathLib {
    using SqrtFunction = num_t (*)(num_t);
    extern SqrtFunction sqrtFunction;
    [[nodiscard]] num_t DefaultSqrt(num_t x);
    [[nodiscard]] num_t GenericSqrt(num_t x);
}

#endif