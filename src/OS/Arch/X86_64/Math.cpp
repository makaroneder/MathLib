#ifdef __x86_64__
#include "ControlRegisters.hpp"
#include "../Arch.hpp"
#include "CPUID.hpp"
#include <Expected.hpp>

bool hasFPU = false;
uint8_t fxsaveRegion[512] __attribute__((aligned(16)));
bool InitMath(void) {
    uintptr_t c;
    uintptr_t d;
    uintptr_t b;
    CPUID(0x1, nullptr, &b, &c, &d);
    bool sseEnabled = false;
    if (d & (1 << (uint8_t)CPUIDBits::D1FXSR)) {
        asm volatile("fxsave %0" :: "m"(fxsaveRegion));
        if (d & (1 << (uint8_t)CPUIDBits::D1FPU)) {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue() || !SetControlRegister(0, tmp.Get() & ~((1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched)))) return false;
            asm volatile("fninit");
            hasFPU = true;
        }
        else {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue() || !SetControlRegister(0, tmp.Get() | (1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched))) return false;
        }
        if (d & (1 << (uint8_t)CPUIDBits::D1SSE)) {
            MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue() || !SetControlRegister(0, (tmp.Get() & ~(1 << (uint8_t)ControlRegister0::Emulation)) | (1 << (uint8_t)ControlRegister0::MonitorCoProcessor))) return false;
            tmp = GetControlRegister(4);
            if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | (1 << (uint8_t)ControlRegister4::FXSaveOSSupport) | (1 << (uint8_t)ControlRegister4::UnmaskedSIMDFloatExceptionsOSSupport))) return false;
            sseEnabled = true;
        }
    }
    if (c & (1 << (uint8_t)CPUIDBits::C1XSave)) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | (1 << (uint8_t)ControlRegister4::XSaveEnable))) return false;
        if (sseEnabled && c & (1 << (uint8_t)CPUIDBits::C1AVX)) asm volatile (
            "xor %%rcx, %%rcx\n"
            "xgetbv\n"
            "or $0b111, %%eax\n"
            "xsetbv" ::: "rax", "rcx", "rdx"
        );
    }
    return true;
}
MathLib::num_t ArchSqrt(MathLib::num_t x) {
    if (hasFPU) {
        MathLib::num_t ret;
        asm volatile (
            "fldt %1\n"
            "fsqrt\n"
            "fstpt %0" : "=m"(ret) : "m"(x) : "st"
        );
        return ret;
    }
    else return GenericSqrt(x);
}
MathLib::num_t ArchInversedTan2(MathLib::num_t y, MathLib::num_t x) {
    if (hasFPU) {
        MathLib::num_t ret;
        asm volatile (
            "fldt %2\n"
            "fldt %1\n"
            "fpatan\n"
            "fstpt %0" : "=m"(ret) : "m"(x), "m"(y) : "st"
        );
        return ret;
    }
    else return GenericInversedTan2(y, x);
}
MathLib::num_t ArchLn(MathLib::num_t x) {
    if (hasFPU) {
        MathLib::num_t ret;
        asm volatile (
            "fldln2\n"
            "fldt %1\n"
            "fyl2x\n"
            "fstpt %0" : "=m"(ret) : "m"(x) : "st"
        );
        return ret;
    }
    else return GenericLn(x);
}
MathLib::num_t ArchExp(MathLib::num_t x) {
    // TODO:
    return GenericExp(x);
}

#endif