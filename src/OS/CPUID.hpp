#ifndef CPUID_H
#define CPUID_H
#include <stdint.h>

enum class CPUIDBits : uint8_t {
    DFPU = 0,
    DFXSR = 24,
    DSSE,
    CXSave,
    CAVX = 28,
};
void CPUID(uintptr_t reg, uintptr_t* a, uintptr_t* b, uintptr_t* c, uintptr_t* d);

#endif