#ifdef __x86_64__
#include "CPUID.hpp"
#include <cpuid.h>

void CPUID(uintptr_t reg, uintptr_t* a, uintptr_t* b, uintptr_t* c, uintptr_t* d) {
    uintptr_t reserved = 0;
    __cpuid(reg, a ? *a : reserved, b ? *b : reserved, c ? *c : reserved, d ? *d : reserved);
}

#endif