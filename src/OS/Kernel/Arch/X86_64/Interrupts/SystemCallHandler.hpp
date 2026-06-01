#ifdef __x86_64__
#ifndef SystemCallHandler_H
#define SystemCallHandler_H
#include "InterruptDevice.hpp"
#include "../../../SystemCall.hpp"

struct SystemCallHandler : InterruptDevice {
    SystemCallHandler(void);
    virtual ~SystemCallHandler(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
};

#endif
#endif