#ifdef __x86_64__
#include "GeneralProtectionFault.hpp"
#include "SelectorErrorCode.hpp"
#include <String.hpp>

GeneralProtectionFault::GeneralProtectionFault(void) : Exception(Type::GeneralProtectionFault) {}
MathLib::String GeneralProtectionFault::GetPanicMessage(uintptr_t error) const {
    return MathLib::String("General protection fault detected\n") + ((SelectorErrorCode*)&error)->ToString() + '\n';
}

#endif