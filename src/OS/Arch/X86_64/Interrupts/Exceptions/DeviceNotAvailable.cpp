#ifdef __x86_64__
#include "DeviceNotAvailable.hpp"

DeviceNotAvailable::DeviceNotAvailable(void) : Exception(Type::DeviceNotAvailable) {}
MathLib::String DeviceNotAvailable::GetPanicMessage(uintptr_t) const {
    return "FPU/MMX/SSE instruction used before initialization\n";
}

#endif