#ifdef __x86_64__
#ifndef SIMDFloatException_H
#define SIMDFloatException_H
#include "Exception.hpp"

struct SIMDFloatException : Exception {
    SIMDFloatException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif