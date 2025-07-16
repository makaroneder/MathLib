#ifdef __x86_64__
#ifndef DeviceNotAvailable_H
#define DeviceNotAvailable_H
#include "Exception.hpp"

struct DeviceNotAvailable : Exception {
    DeviceNotAvailable(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif