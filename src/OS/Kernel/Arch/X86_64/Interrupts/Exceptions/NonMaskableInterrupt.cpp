#ifdef __x86_64__
#include "NonMaskableInterrupt.hpp"

NonMaskableInterrupt::NonMaskableInterrupt(void) : Exception(Type::NonMaskableInterrupt) {}
MathLib::String NonMaskableInterrupt::GetPanicMessage(uintptr_t) const {
    // TODO: Check system control port A (0x92) and B (0x61)
    return "Non maskable interrupt detected\n";
}

#endif