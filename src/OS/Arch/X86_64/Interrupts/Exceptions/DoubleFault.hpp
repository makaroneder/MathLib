#ifndef DoubleFault_H
#define DoubleFault_H
#include "Exception.hpp"

struct DoubleFault : Exception {
    DoubleFault(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif