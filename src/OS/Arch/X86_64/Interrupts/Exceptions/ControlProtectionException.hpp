#ifdef __x86_64__
#ifndef ControlProtectionException_H
#define ControlProtectionException_H
#include "Exception.hpp"

struct ControlProtectionException : Exception {
    ControlProtectionException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif