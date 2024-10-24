#ifdef __x86_64__
#include "SIMDFloatException.hpp"

SIMDFloatException::SIMDFloatException(void) : Exception(Type::SIMDFloatException) {}
String SIMDFloatException::GetPanicMessage(uintptr_t) const {
    // TODO: Check MXCSR
    return "SIMD float exception detected\n";
}

#endif