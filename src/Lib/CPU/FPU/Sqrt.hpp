#ifdef __x86_64__
#ifndef MathLib_CPU_FPU_Sqrt_H
#define MathLib_CPU_FPU_Sqrt_H
#include "../../Typedefs.hpp"

namespace MathLib {
    [[nodiscard]] num_t FPUSqrt(num_t x);
}

#endif
#endif