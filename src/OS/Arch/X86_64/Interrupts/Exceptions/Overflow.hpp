#ifndef Overflow_H
#define Overflow_H
#include "Exception.hpp"

struct Overflow : Exception {
    Overflow(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif