#ifndef GeneralProtectionFault_H
#define GeneralProtectionFault_H
#include "Exception.hpp"

struct GeneralProtectionFault : Exception {
    GeneralProtectionFault(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif