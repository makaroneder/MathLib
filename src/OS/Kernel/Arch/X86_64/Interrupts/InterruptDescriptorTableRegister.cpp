#ifdef __x86_64__
#include "InterruptDescriptorTableRegister.hpp"

InterruptDescriptorTableRegister::InterruptDescriptorTableRegister(InterruptDescriptorTable* base) : limit(sizeof(InterruptDescriptorTable) - 1), base(base) {}

#endif