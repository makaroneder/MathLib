#ifdef __x86_64__
#ifndef InterruptDevice_H
#define InterruptDevice_H
#include "../Registers.hpp"

struct InterruptDevice : MathLib::Allocatable {
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) = 0;
};
enum class IRQ : uint8_t {
    PIT = 0,
    FirstPS2,
    Cascade,
    COM2,
    COM1,
    LPT2,
    Floppy,
    LPT1,
    RTC,
    SecondPS2 = 12,
    FPU,
    ATA1,
    ATA2,
};
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device);
[[nodiscard]] bool RegisterIRQDevice(IRQ irq, InterruptDevice* device);

#endif
#endif