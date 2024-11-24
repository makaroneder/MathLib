#ifdef __x86_64__
#include "DivisionError.hpp"

DivisionError::DivisionError(void) : Exception(Type::DivisionError) {}
MathLib::String DivisionError::GetPanicMessage(uintptr_t) const {
    return "Division error detected\n";
}
void DivisionError::Trigger(Registers regs) {
    // TODO: Set r11-15
    asm volatile (
        "mov %1, %%r8\n"
        "mov %2, %%r9\n"
        "mov %3, %%r10\n"
        "idiv %0" : : "r"(0), "r"(regs.r8), "r"(regs.r9), "r"(regs.r10),
        "a"(regs.a), "b"(regs.b), "c"(regs.c), "d"(regs.d), "S"(regs.si), "D"(regs.di) :
        "r8", "r9", "r10"
    );
}

#endif