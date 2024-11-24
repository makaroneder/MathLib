#ifdef __x86_64__
#ifndef InterruptTimer_H
#define InterruptTimer_H
#include "../../Time.hpp"
#include "Interrupts/Interrupts.hpp"
#include "FunctionPointer.hpp"

struct InterruptTimer : Timer, InterruptDevice {
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    void SetFunction(const MathLib::FunctionPointer<void, Registers*>& func);

    private:
    MathLib::FunctionPointer<void, Registers*> function;
};

#endif
#endif