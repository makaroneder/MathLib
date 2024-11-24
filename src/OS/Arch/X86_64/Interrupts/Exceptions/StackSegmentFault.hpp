#ifdef __x86_64__
#ifndef StackSegmentFault_H
#define StackSegmentFault_H
#include "Exception.hpp"

struct StackSegmentFault : Exception {
    StackSegmentFault(void);
    virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif