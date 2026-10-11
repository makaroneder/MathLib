#ifdef __x86_64__
#include "Ln.hpp"

namespace MathLib {
    num_t FPULn(num_t x) {
        MathLib::num_t ret;
        asm volatile (
            "fldln2\n"
            "fldt %1\n"
            "fyl2x\n"
            "fstpt %0" : "=m"(ret) : "m"(x) : "st"
        );
        return ret;
    }
}

#endif