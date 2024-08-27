#include "DebugException.hpp"

DebugException::DebugException(void) : Exception(Type::DebugException) {}
String DebugException::GetPanicMessage(uintptr_t) const {
    return "Debug exception detected\n";
}