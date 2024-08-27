#ifndef HypervisorInjectionException_H
#define HypervisorInjectionException_H
#include "Exception.hpp"

struct HypervisorInjectionException : Exception {
    HypervisorInjectionException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif