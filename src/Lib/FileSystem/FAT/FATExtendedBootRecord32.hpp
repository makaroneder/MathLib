#ifndef MathLib_FileSystem_FAT_ExtendedBootRecord32_H
#define MathLib_FileSystem_FAT_ExtendedBootRecord32_H
#include "FATExtendedBootRecord.hpp"

namespace MathLib {
    struct FATExtendedBootRecord32 {
        uint32_t sectorsPerFAT;
        uint16_t flags;
        uint16_t version;
        uint32_t rootCluster;
        uint16_t fsInfoCluster;
        uint16_t backupBootSectorCluster;
        uint32_t reserved[3];
        FATExtendedBootRecord ebr;

        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif