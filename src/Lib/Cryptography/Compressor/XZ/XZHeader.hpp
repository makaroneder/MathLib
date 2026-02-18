#ifndef MathLib_Cryptography_Compressor_XZ_Header_H
#define MathLib_Cryptography_Compressor_XZ_Header_H
#include "XZFlags.hpp"

namespace MathLib {
    struct XZHeader {
        static constexpr uint16_t expectedSignature[] = { 0x37fd, 0x587a, 0x005a, };
        uint16_t signature[3];
        XZFlags flags;
        uint32_t crc32;

        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif