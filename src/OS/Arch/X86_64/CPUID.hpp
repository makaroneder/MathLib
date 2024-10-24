#ifdef __x86_64__
#ifndef CPUID_H
#define CPUID_H
#include <stdint.h>

enum class CPUIDBits : uint8_t {
    D1FPU = 0,
    D1MSR = 5,
    B7SMEP = 7,
    B7SMAP = 20,
    D1FXSR = 24,
    D1SSE,
    C1XSave,
    C1AVX = 28,
};
void CPUID(uintptr_t reg, uintptr_t* a, uintptr_t* b, uintptr_t* c, uintptr_t* d);

#endif
#endif