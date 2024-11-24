#ifdef __x86_64__
#include "Cascade.hpp"
#include <Host.hpp>

Cascade::Cascade(void) {
    RegisterInterruptDevice(GetIRQBase() + 2, this);
}
Cascade::~Cascade(void) {
    RegisterInterruptDevice(GetIRQBase() + 2, nullptr);
}
void Cascade::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    MathLib::Panic("Cascade interrupt raised");
}

#endif