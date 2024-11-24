#ifdef __x86_64__
#include "InterruptTimer.hpp"

void InterruptTimer::OnInterrupt(uintptr_t, Registers* regs, uintptr_t) {
    time += 1 / GetFrequency();
    if (function.GetFunction()) function(regs);
}
void InterruptTimer::SetFunction(const MathLib::FunctionPointer<void, Registers*>& func) {
    function = func;
}

#endif