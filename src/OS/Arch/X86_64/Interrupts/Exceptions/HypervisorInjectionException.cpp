#include "HypervisorInjectionException.hpp"

HypervisorInjectionException::HypervisorInjectionException(void) : Exception(Type::HypervisorInjectionException) {}
String HypervisorInjectionException::GetPanicMessage(uintptr_t) const {
    return "Hypervisor injection exception detected\n";
}