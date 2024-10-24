#ifdef __x86_64__
#ifndef Breakpoint_H
#define Breakpoint_H
#include "Exception.hpp"

struct Breakpoint : Exception {
    Breakpoint(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif