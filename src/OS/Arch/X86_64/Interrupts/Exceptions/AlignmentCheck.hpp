#ifdef __x86_64__
#ifndef AlignmentCheck_H
#define AlignmentCheck_H
#include "Exception.hpp"

struct AlignmentCheck : Exception {
    AlignmentCheck(void);
    static bool Enable(void);
    virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif