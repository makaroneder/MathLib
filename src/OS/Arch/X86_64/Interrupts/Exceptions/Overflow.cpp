#ifdef __x86_64__
#include "Overflow.hpp"

Overflow::Overflow(void) : Exception(Type::Overflow) {}
String Overflow::GetPanicMessage(uintptr_t) const {
    return "Overflow detected\n";
}

#endif