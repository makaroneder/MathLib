#ifdef __x86_64__
#include "PageFault.hpp"

PageFault::PageFault(void) : Exception(Type::PageFault) {}
String PageFault::GetPanicMessage(uintptr_t) const {
    // TODO: Decode error code and fault address from CR2
    return "Page fault detected\n";
}

#endif