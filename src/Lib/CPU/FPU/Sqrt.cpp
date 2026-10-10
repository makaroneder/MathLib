#ifdef __x86_64__
#include "Sqrt.hpp"

namespace MathLib {
    num_t FPUSqrt(num_t x) {
        MathLib::num_t ret;
        asm volatile (
            "fldt %1\n"
            "fsqrt\n"
            "fstpt %0" : "=m"(ret) : "m"(x) : "st"
        );
        return ret;
    }
}

#endif