#ifndef Interrupts_H
#define Interrupts_H
#include "../Registers.hpp"
#include <Allocatable.hpp>

struct InterruptDevice : Allocatable {
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) = 0;
};
uint8_t GetIRQBase(void);
void SetInterrupts(bool value);
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device);
void InitInterrupts(uint8_t irqBase, uint8_t codeSegment);

#endif