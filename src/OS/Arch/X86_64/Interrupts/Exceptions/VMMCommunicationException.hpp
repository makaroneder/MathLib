#ifndef VMMCommunicationException_H
#define VMMCommunicationException_H
#include "Exception.hpp"

struct VMMCommunicationException : Exception {
    VMMCommunicationException(void);
    virtual String GetPanicMessage(uintptr_t error) const override;
};

#endif