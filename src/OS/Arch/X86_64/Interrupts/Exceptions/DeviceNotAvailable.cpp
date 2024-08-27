#include "DeviceNotAvailable.hpp"

DeviceNotAvailable::DeviceNotAvailable(void) : Exception(Type::DeviceNotAvailable) {}
String DeviceNotAvailable::GetPanicMessage(uintptr_t) const {
    return "FPU/MMX/SSE instruction used before initialization\n";
}