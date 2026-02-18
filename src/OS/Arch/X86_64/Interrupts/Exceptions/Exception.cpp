#ifdef __x86_64__
#include "Exception.hpp"
#include "../../Stack.hpp"
#include <Host.hpp>

Exception::Exception(Type type) : type(type) {
    RegisterInterruptDevice((uintptr_t)type, this);
}
Exception::~Exception(void) {
    RegisterInterruptDevice((uintptr_t)type, nullptr);
}
void Exception::OnInterrupt(uintptr_t, Registers* regs, uintptr_t error) {
    MathLib::Panic(GetPanicMessage(error) + "State: " + regs->ToX86State().ToString() + "\nStack trace: " + StackFrameToString((const StackFrame*)regs->bp));
}

#endif