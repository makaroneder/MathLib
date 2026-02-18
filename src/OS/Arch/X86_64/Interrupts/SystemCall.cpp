#ifdef __x86_64__
#include "SystemCall.hpp"

SystemCall::SystemCall(void) {
    RegisterInterruptDevice(0x80, this);
}
SystemCall::~SystemCall(void) {
    RegisterInterruptDevice(0x80, nullptr);
}
void SystemCall::OnInterrupt(uintptr_t, Registers* regs, uintptr_t) {
    regs->a = OnSystemCall(regs->a, (void*)regs->b, (void*)regs->c, (void*)regs->d);
}

#endif