#ifdef __x86_64__
#ifndef MachineCheck_H
#define MachineCheck_H
#include "Exception.hpp"

struct MachineCheck : Exception {
    MachineCheck(void);
    [[nodiscard]] static bool Enable(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif