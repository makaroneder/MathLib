#ifdef __x86_64__
#include "DebugException.hpp"

DebugException::DebugException(void) : Exception(Type::DebugException) {}
MathLib::String DebugException::GetPanicMessage(uintptr_t) const {
    return "Debug exception detected\n";
}

#endif