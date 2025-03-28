#ifdef __x86_64__
#ifndef SystemCall_H
#define SystemCall_H
#include "InterruptDevice.hpp"
#include "../../../SystemCallHandler.hpp"

struct SystemCall : InterruptDevice, SystemCallHandler {
    SystemCall(void);
    virtual ~SystemCall(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
};

#endif
#endif