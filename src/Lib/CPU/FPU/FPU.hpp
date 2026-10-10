#ifdef __x86_64__
#ifndef MathLib_CPU_FPU_H
#define MathLib_CPU_FPU_H

namespace MathLib {
    extern bool fpu;
    extern bool waitForFPU;

    void CheckFPU(void);
}

#endif
#endif