#ifdef __x86_64__
#ifndef SegmentNotPresent_H
#define SegmentNotPresent_H
#include "Exception.hpp"

struct SegmentNotPresent : Exception {
    SegmentNotPresent(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif