#ifndef AlignmentCheck_H
#define AlignmentCheck_H
#include "Exception.hpp"

struct AlignmentCheck : Exception {
    AlignmentCheck(void);
    static void Enable(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif