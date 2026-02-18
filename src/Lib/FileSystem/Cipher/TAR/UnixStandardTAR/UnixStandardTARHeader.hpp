#ifndef MathLib_FileSystem_Cipher_TAR_UnixStandard_Header_H
#define MathLib_FileSystem_Cipher_TAR_UnixStandard_Header_H
#include "../ExtendedTARHeader.hpp"

namespace MathLib {
    struct UnixStandardTARHeader : ExtendedTARHeader {
        static constexpr const char* expectedSignature = "ustar";
        static constexpr const char* expectedVersion = "00";
        char pathPrefix[155];
        uint32_t reserved[3];

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] String GetName(void) const;
        [[nodiscard]] bool Create(const Sequence<char>& path, size_t dataSize);
    } __attribute__((packed));
}

#endif