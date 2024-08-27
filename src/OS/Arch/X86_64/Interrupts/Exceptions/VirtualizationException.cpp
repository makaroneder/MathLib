#include "VirtualizationException.hpp"

VirtualizationException::VirtualizationException(void) : Exception(Type::VirtualizationException) {}
String VirtualizationException::GetPanicMessage(uintptr_t) const {
    return "Virtualization exception detected\n";
}