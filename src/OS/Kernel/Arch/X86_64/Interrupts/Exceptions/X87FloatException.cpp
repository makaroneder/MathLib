#ifdef __x86_64__
#include "X87FloatException.hpp"

X87FloatException::X87FloatException(void) : Exception(Type::X87FloatException) {}
MathLib::String X87FloatException::GetPanicMessage(uintptr_t) const {
    // TODO: Check x87 status word register
    return "X87 float exception detected\n";
}

#endif