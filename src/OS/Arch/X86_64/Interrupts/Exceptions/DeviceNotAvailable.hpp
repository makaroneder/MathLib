#ifdef __x86_64__
#ifndef DeviceNotAvailable_H
#define DeviceNotAvailable_H
#include "Exception.hpp"

struct DeviceNotAvailable : Exception {
    DeviceNotAvailable(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif