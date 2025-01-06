#ifdef __x86_64__
#ifndef Interrupts_H
#define Interrupts_H
#include "../Registers.hpp"

struct InterruptDevice : MathLib::Allocatable {
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) = 0;
};
[[nodiscard]] uint8_t GetIRQBase(void);
void SetInterrupts(bool value);
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device);
[[nodiscard]] bool InitInterrupts(uint8_t irqBase, uint8_t codeSegment);

#endif
#endif