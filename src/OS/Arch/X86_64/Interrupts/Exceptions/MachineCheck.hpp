#ifdef __x86_64__
#ifndef MachineCheck_H
#define MachineCheck_H
#include "Exception.hpp"

struct MachineCheck : Exception {
    MachineCheck(void);
    static bool Enable(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif