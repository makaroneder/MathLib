#ifdef __x86_64__
#include "HypervisorInjectionException.hpp"

HypervisorInjectionException::HypervisorInjectionException(void) : Exception(Type::HypervisorInjectionException) {}
MathLib::String HypervisorInjectionException::GetPanicMessage(uintptr_t) const {
    return "Hypervisor injection exception detected\n";
}

#endif