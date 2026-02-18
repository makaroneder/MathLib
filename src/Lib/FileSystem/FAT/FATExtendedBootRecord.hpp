#ifndef MathLib_FileSystem_FAT_ExtendedBootRecord_H
#define MathLib_FileSystem_FAT_ExtendedBootRecord_H
#include <stdint.h>

namespace MathLib {
    struct FATExtendedBootRecord {
        enum class Type : uint8_t {
            NoID = 0x28,
            Normal,
        };
        uint8_t drive;
        uint8_t flags;
        Type type;
        char volumeID[4];
        char volumeLabel[11];
        char systemID[8];

        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif