#ifndef MathLib_FileSystem_FAT_LongFileName_H
#define MathLib_FileSystem_FAT_LongFileName_H
#include <stdint.h>

namespace MathLib {
    struct FATLongFileName {
        static constexpr uint8_t expectedSignature = 0x0f;
        uint8_t order;
        char16_t name1[5];
        uint8_t signature;
        uint8_t type;
        uint8_t checksum;
        char16_t name2[6];
        uint16_t reserved;
        char16_t name3[2];

        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif