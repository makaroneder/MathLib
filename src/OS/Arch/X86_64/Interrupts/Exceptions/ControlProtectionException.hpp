#ifdef __x86_64__
#ifndef ControlProtectionException_H
#define ControlProtectionException_H
#include "Exception.hpp"

struct ControlProtectionException : Exception {
    ControlProtectionException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif