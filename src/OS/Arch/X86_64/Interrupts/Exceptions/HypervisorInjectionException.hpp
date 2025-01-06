#ifdef __x86_64__
#ifndef HypervisorInjectionException_H
#define HypervisorInjectionException_H
#include "Exception.hpp"

struct HypervisorInjectionException : Exception {
    HypervisorInjectionException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif