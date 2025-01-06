#ifdef __x86_64__
#ifndef VMMCommunicationException_H
#define VMMCommunicationException_H
#include "Exception.hpp"

struct VMMCommunicationException : Exception {
    VMMCommunicationException(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif