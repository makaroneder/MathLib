#ifdef __x86_64__
#ifndef DivisionError_H
#define DivisionError_H
#include "Exception.hpp"

struct DivisionError : Exception {
    DivisionError(void);
    static void Trigger(Registers regs);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif