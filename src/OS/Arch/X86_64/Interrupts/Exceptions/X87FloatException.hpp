#ifdef __x86_64__
#ifndef X87FloatException_H
#define X87FloatException_H
#include "Exception.hpp"

struct X87FloatException : Exception {
    X87FloatException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif