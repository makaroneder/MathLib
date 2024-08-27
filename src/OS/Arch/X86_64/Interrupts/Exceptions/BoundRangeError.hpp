#ifndef BoundRangeError_H
#define BoundRangeError_H
#include "Exception.hpp"

struct BoundRangeError : Exception {
    BoundRangeError(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif