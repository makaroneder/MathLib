#ifndef MathLib_FileSystem_Cipher_TAR_GNU_Header_H
#define MathLib_FileSystem_Cipher_TAR_GNU_Header_H
#include "GNUTARSparseFileData.hpp"
#include "../ExtendedTARHeader.hpp"

namespace MathLib {
    struct GNUTARHeader : ExtendedTARHeader {
        static constexpr const char* expectedSignature = "ustar ";
        static constexpr const char* expectedVersion = " ";
        char lastAccessTime[12];
        char lastChangeTime[12];
        char offset[12];
        char longNames[4];
        uint8_t reserved1;
        GNUTARSparseFileData sparse[4];
        bool extended;
        char gnuSize[12];
        uint8_t reserved2[17];

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool Create(const Sequence<char>& path, size_t dataSize);
    } __attribute__((packed));
}

#endif