#ifdef __x86_64__
#include "DoubleFault.hpp"

DoubleFault::DoubleFault(void) : Exception(Type::DoubleFault) {}
MathLib::String DoubleFault::GetPanicMessage(uintptr_t) const {
    return "Double fault detected\n";
}

#endif