#ifdef __x86_64__
#ifndef Cascade_H
#define Cascade_H
#include "InterruptDevice.hpp"

struct Cascade : InterruptDevice {
    Cascade(void);
    virtual ~Cascade(void) override;
    virtual void OnInterrupt(uintptr_t, Registers*, uintptr_t) override;
};

#endif
#endif