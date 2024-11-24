#ifdef __x86_64__
#ifndef PageFault_H
#define PageFault_H
#include "Exception.hpp"

struct PageFault : Exception {
    PageFault(void);
    virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif