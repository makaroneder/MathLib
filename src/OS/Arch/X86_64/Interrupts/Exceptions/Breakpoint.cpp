#ifdef __x86_64__
#include "Breakpoint.hpp"

Breakpoint::Breakpoint(void) : Exception(Type::Breakpoint) {}
String Breakpoint::GetPanicMessage(uintptr_t) const {
    return "Breakpoint detected\n";
}

#endif