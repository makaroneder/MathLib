#ifdef __x86_64__
#include "ControlProtectionException.hpp"

ControlProtectionException::ControlProtectionException(void) : Exception(Type::ControlProtectionException) {}
String ControlProtectionException::GetPanicMessage(uintptr_t) const {
    return "Control protection exception detected\n";
}

#endif