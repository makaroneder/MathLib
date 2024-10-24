#ifdef __x86_64__
#ifndef DebugException_H
#define DebugException_H
#include "Exception.hpp"

struct DebugException : Exception {
    DebugException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif