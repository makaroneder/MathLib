#ifdef __x86_64__
#ifndef VirtualizationException_H
#define VirtualizationException_H
#include "Exception.hpp"

struct VirtualizationException : Exception {
    VirtualizationException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif