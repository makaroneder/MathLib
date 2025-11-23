#ifndef MathLib_FileSystem_Cipher_TAR_ExtendedHeader_H
#define MathLib_FileSystem_Cipher_TAR_ExtendedHeader_H
#include "TARHeader.hpp"

namespace MathLib {
    struct ExtendedTARHeader : TARHeader {
        char signature[6];
        char version[2];
        char ownerName[32];
        char groupName[32];
        char deviceMajor[8];
        char deviceMinor[8];

        [[nodiscard]] bool IsValid(const char* expectedSignature, const char* expectedVersion) const;
        [[nodiscard]] bool Create(const Sequence<char>& path, size_t dataSize, const char* expectedSignature, const char* expectedVersion);
    } __attribute__((packed));
}

#endif