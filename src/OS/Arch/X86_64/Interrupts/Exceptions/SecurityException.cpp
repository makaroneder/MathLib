#ifdef __x86_64__
#include "SecurityException.hpp"

SecurityException::SecurityException(void) : Exception(Type::SecurityException) {}
MathLib::String SecurityException::GetPanicMessage(uintptr_t) const {
    return "Security exception detected\n";
}

#endif