#ifndef VirtualizationException_H
#define VirtualizationException_H
#include "Exception.hpp"

struct VirtualizationException : Exception {
    VirtualizationException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif