#ifdef __x86_64__
#include "FPU.hpp"
#include "../CPUID.hpp"

namespace MathLib {
    bool fpu = false;
    bool waitForFPU = true;
    bool checkedFPU = false;

    void CheckFPU(void) {
        if (checkedFPU || waitForFPU) return;
        uintptr_t tmp;
        CPUID(0x1, nullptr, nullptr, nullptr, &tmp);
        fpu = tmp & (1 << (uint8_t)CPUIDBits::D1FPU);
        checkedFPU = true;
    }
}

#endif