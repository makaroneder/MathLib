#ifndef MathLib_FileSystem_Cipher_TAR_Header_H
#define MathLib_FileSystem_Cipher_TAR_Header_H
#include "../../../Typedefs.hpp"

namespace MathLib {
    struct TARHeader {
        enum class Type : char {
            Normal = '0',
            HardLink,
            UstarSymbolicLink,
            UstarCharDevice,
            UstarBlockDevice,
            UstarDirectory,
            UstarPipe,
            UstarContiguousFile,
            GNUNormal = UstarContiguousFile,
            GNUDirectory = 'D',
            GNULongLinkName1,
            GNULongLinkName2,
            GNUPrevFileContinuation = 'M',
            GNUFilesToRename,
            GNUSparse = 'S',
            GNUHeaderName = 'V',
            UstarGlobalExtendedHeader = 'g',
            UstarExtendedHeader = 'x',
        };
        char path[100];
        char mode[8];
        char ownerID[8];
        char groupID[8];
        char size[12];
        char lastModificationTime[12];
        char checksum[8];
        Type type;
        char linkedPath[100];

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] size_t GetChecksum(void) const;
        [[nodiscard]] String GetName(void) const;
        [[nodiscard]] size_t GetSize(void) const;
        [[nodiscard]] static size_t PaddSize(size_t size);
        [[nodiscard]] bool Create(const Sequence<char>& path, size_t dataSize);
    } __attribute__((packed));
}

#endif