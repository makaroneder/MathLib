#ifndef MathLib_Cryptography_Compressor_XZ_Footer_H
#define MathLib_Cryptography_Compressor_XZ_Footer_H
#include "XZFlags.hpp"

namespace MathLib {
    struct XZFooter {
        static constexpr uint16_t expectedSignature = 0x5a59;
        uint32_t crc32;
        uint32_t indexSize;
        XZFlags flags;
        uint16_t signature;

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] uint64_t GetIndexSize(void) const;
    } __attribute__((packed));
}

#endif