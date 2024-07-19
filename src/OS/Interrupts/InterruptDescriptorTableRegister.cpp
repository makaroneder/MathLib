#include "InterruptDescriptorTableRegister.hpp"

InterruptDescriptorTableRegister::InterruptDescriptorTableRegister(InterruptDescriptorTable* base) : limit(sizeof(InterruptDescriptorTable) - 1), base(base) {}