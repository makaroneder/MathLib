#ifdef __x86_64__
#ifndef InterruptTimer_H
#define InterruptTimer_H
#include "Interrupts/InterruptDevice.hpp"
#include "FunctionPointer.hpp"
#include "../../Time.hpp"

struct InterruptTimer : Timer, InterruptDevice {
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
};

#endif
#endif