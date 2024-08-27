#ifndef InvalidTSS_H
#define InvalidTSS_H
#include "Exception.hpp"

struct InvalidTSS : Exception {
    InvalidTSS(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif