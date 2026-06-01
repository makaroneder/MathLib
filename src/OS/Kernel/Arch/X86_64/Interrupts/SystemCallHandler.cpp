#ifdef __x86_64__
#include "SystemCallHandler.hpp"

SystemCallHandler::SystemCallHandler(void) {
    RegisterInterruptDevice(0x80, this);
}
SystemCallHandler::~SystemCallHandler(void) {
    RegisterInterruptDevice(0x80, nullptr);
}
void SystemCallHandler::OnInterrupt(uintptr_t, Registers* regs, uintptr_t) {
    regs->a = OnSystemCall((SystemCall)regs->a, (void*)regs->b);
}

#endif