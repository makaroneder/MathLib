#ifdef __x86_64__
#include "BoundRangeError.hpp"

BoundRangeError::BoundRangeError(void) : Exception(Type::BoundRageExceeded) {}
String BoundRangeError::GetPanicMessage(uintptr_t) const {
    return "Bound range error detected\n";
}

#endif