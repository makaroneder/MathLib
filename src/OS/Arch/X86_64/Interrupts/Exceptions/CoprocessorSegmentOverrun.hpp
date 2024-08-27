#ifndef CoprocessorSegmentOverrun_H
#define CoprocessorSegmentOverrun_H
#include "Exception.hpp"

struct CoprocessorSegmentOverrun : Exception {
    CoprocessorSegmentOverrun(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif