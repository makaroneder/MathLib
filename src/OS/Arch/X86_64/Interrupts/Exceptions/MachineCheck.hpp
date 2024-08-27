#ifndef MachineCheck_H
#define MachineCheck_H
#include "Exception.hpp"

struct MachineCheck : Exception {
    MachineCheck(void);
    static void Enable(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif