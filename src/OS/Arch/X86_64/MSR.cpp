#ifdef __x86_64__
#include "MSR.hpp"
#include "CPUID.hpp"
#include <Emulator/Register.hpp>

bool hasMSR = false;
bool HasMSR(void) {
    if (hasMSR) return hasMSR;
    uintptr_t d;
    CPUID(0x1, nullptr, nullptr, nullptr, &d);
    if (d & (1 << (uint8_t)CPUIDBits::D1MSR)) hasMSR = true;
    return hasMSR;
}
Expected<uint64_t> GetMSR(uint32_t msr) {
    if (!HasMSR()) return Expected<uint64_t>();
    uint32_t low = 0;
    uint32_t high = 0;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    Register tmp;
    tmp.Set32(low, false);
    tmp.Set32(high, true);
    return Expected<uint64_t>(tmp.value);
}
bool SetMSR(uint32_t msr, uint64_t value) {
    if (!HasMSR()) return false;
    const Register tmp = Register(value);
    asm volatile("wrmsr" : : "a"(tmp.Get32(false)), "d"(tmp.Get32(true)), "c"(msr));
    return true;
}

#endif