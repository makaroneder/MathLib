#ifdef __x86_64__
#ifndef InvalidOpcode_H
#define InvalidOpcode_H
#include "Exception.hpp"

struct InvalidOpcode : Exception {
    InvalidOpcode(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif