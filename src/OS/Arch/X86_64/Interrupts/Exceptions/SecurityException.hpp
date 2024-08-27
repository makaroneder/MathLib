#ifndef SecurityException_H
#define SecurityException_H
#include "Exception.hpp"

struct SecurityException : Exception {
    SecurityException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif