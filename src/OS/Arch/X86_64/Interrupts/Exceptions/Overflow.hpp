#ifdef __x86_64__
#ifndef Overflow_H
#define Overflow_H
#include "Exception.hpp"

struct Overflow : Exception {
    Overflow(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif