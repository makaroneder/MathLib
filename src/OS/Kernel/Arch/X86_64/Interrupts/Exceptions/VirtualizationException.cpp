#ifdef __x86_64__
#include "VirtualizationException.hpp"

VirtualizationException::VirtualizationException(void) : Exception(Type::VirtualizationException) {}
MathLib::String VirtualizationException::GetPanicMessage(uintptr_t) const {
    return "Virtualization exception detected\n";
}

#endif