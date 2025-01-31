#ifdef __x86_64__
#ifndef PageFault_H
#define PageFault_H
#include "Exception.hpp"

struct PageFaultErrorCode {
    bool present : 1;
    bool write : 1;
    bool user : 1;
    bool reservedWrite : 1;
    bool instructionFetch : 1;
    bool protectionKey : 1;
    bool shadowStack : 1;
    bool softwareGuardExtensions : 1;

    [[nodiscard]] MathLib::String ToString(const MathLib::String& padding = "") const;
} __attribute__((packed));
struct PageFault : Exception {
    PageFault(void);
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const override;
};

#endif
#endif