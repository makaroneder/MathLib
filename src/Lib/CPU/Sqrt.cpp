#include "Sqrt.hpp"
#include "FPU/FPU.hpp"
#include "../Host.hpp"
#include "FPU/Sqrt.hpp"

namespace MathLib {
    SqrtFunction sqrtFunction = &DefaultSqrt;
    num_t DefaultSqrt(num_t x) {
        #ifdef __x86_64__
        if (waitForFPU) return GenericSqrt(x);
        CheckFPU();
        sqrtFunction = fpu ? &FPUSqrt : &GenericSqrt;
        return sqrtFunction(x);
        #endif
        sqrtFunction = &GenericSqrt;
        return sqrtFunction(x);
    }
    num_t GenericSqrt(num_t x) {
        if (x < 0) return nan;
        if (FloatsEqual<num_t>(x, 0)) return 0;
        num_t ret = x;
        num_t prev;
        do {
            prev = ret;
            ret -= (ret * ret - x) / (2 * ret);
        } while (Abs(prev - ret) >= eps);
        return ret;
    }
}