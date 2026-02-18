#ifndef MathLib_Cryptography_Compressor_XZ_Flags_H
#define MathLib_Cryptography_Compressor_XZ_Flags_H
#include "../../../ExternArray.hpp"

namespace MathLib {
    struct XZFlags {
        enum class CheckType : uint8_t {
            None = 0,
            CRC32,
            CRC64 = 4,
            SHA256 = 10,
        };
        uint8_t flags1;
        uint8_t checkType : 4;
        uint8_t flags2 : 4;

        [[nodiscard]] uint8_t GetSizeOfCheck(void) const;
        [[nodiscard]] ExternArray<uint8_t> GetFlags(void) const;
        [[nodiscard]] bool operator==(const XZFlags& other) const;
        [[nodiscard]] bool operator!=(const XZFlags& other) const;
    } __attribute__((packed));
}

#endif