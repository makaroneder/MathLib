#ifndef SIMDFloatException_H
#define SIMDFloatException_H
#include "Exception.hpp"

struct SIMDFloatException : Exception {
    SIMDFloatException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif