#include "Cascade.hpp"
#include <Host.hpp>

Cascade::Cascade(void) {
    RegisterInterruptDevice(GetIRQBase() + 2, this);
}
Cascade::~Cascade(void) {
    RegisterInterruptDevice(GetIRQBase() + 2, nullptr);
}
void Cascade::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    Panic("Cascade interrupt raised");
}