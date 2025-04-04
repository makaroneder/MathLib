#ifdef __x86_64__
#ifndef DoubleFault_H
#define DoubleFault_H
#include "Exception.hpp"

struct DoubleFault : Exception {
    DoubleFault(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif