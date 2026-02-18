#ifdef __x86_64__
#include "VMMCommunicationException.hpp"

VMMCommunicationException::VMMCommunicationException(void) : Exception(Type::VMMCommunicationException) {}
MathLib::String VMMCommunicationException::GetPanicMessage(uintptr_t) const {
    return "VMM communication exception detected\n";
}

#endif