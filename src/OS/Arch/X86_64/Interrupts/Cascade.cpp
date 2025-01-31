#ifdef __x86_64__
#include "Cascade.hpp"
#include <Host.hpp>

Cascade::Cascade(void) {
    if (!RegisterIRQDevice(IRQ::Cascade, this)) MathLib::Panic("Failed to register IRQ");
}
Cascade::~Cascade(void) {
    if (!RegisterIRQDevice(IRQ::Cascade, nullptr)) MathLib::Panic("Failed to register IRQ");
}
void Cascade::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    MathLib::Panic("Cascade interrupt raised");
}

#endif