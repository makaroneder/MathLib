#ifndef MathLib_FileSystem_Cipher_Ar_Header_H
#define MathLib_FileSystem_Cipher_Ar_Header_H
#include "../../../Typedefs.hpp"

namespace MathLib {
    struct ArHeader {
        static constexpr uint16_t expectedSignature = 0x0a60;
        char name[16];
        char modificationTimestamp[12];
        char ownerID[6];
        char groupID[6];
        char mode[8];
        char size[10];
        uint16_t signature;

        ArHeader(void);
        ArHeader(const Sequence<char>& path, size_t dataSize);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] String GetName(void) const;
        [[nodiscard]] size_t GetSize(void) const;
    } __attribute__((packed));
}

#endif