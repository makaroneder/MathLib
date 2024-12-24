#ifndef MathLib_FileSystem_MathFS_BootSector_H
#define MathLib_FileSystem_MathFS_BootSector_H
#include <stdint.h>

namespace MathLib {
    struct MathFSBootSector {
        static constexpr const uint8_t expectedSignature1 = 'M';
        static constexpr const uint16_t expectedSignature2 = 0xaa55;
        uint8_t code[501];
        uint8_t signature1;
        uint64_t root;
        uint16_t signature2;

        MathFSBootSector(void);
        MathFSBootSector(uint64_t root);
        bool IsValid(void) const;
    } __attribute__((packed));
}

#endif