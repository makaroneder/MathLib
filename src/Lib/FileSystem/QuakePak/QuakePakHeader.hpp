#ifndef MathLib_FileSystem_QuakePak_Header_H
#define MathLib_FileSystem_QuakePak_Header_H
#include <stdint.h>

namespace MathLib {
    struct QuakePakHeader {
        static constexpr const char* expectedSignature = "PACK";
        char signature[4];
        uint32_t fileTablePosition;
        uint32_t fileTableSize;

        QuakePakHeader(void);
        QuakePakHeader(uint32_t fileTablePosition);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif