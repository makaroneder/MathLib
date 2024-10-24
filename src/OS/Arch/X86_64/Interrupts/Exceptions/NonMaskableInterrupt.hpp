#ifdef __x86_64__
#ifndef NonMaskableInterrupt_H
#define NonMaskableInterrupt_H
#include "Exception.hpp"

struct NonMaskableInterrupt : Exception {
    NonMaskableInterrupt(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif