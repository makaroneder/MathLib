#include "GeneralProtectionFault.hpp"
#include <String.hpp>

GeneralProtectionFault::GeneralProtectionFault(void) : Exception(Type::GeneralProtectionFault) {}
String GeneralProtectionFault::GetPanicMessage(uintptr_t error) const {
    return String("General protection fault detected\nSegment selector: 0x") + ToString(error, 16) + '\n';
}