#ifdef __x86_64__
#ifndef CoprocessorSegmentOverrun_H
#define CoprocessorSegmentOverrun_H
#include "Exception.hpp"

struct CoprocessorSegmentOverrun : Exception {
    CoprocessorSegmentOverrun(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif