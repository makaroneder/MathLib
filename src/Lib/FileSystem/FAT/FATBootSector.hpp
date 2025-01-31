#ifndef MathLib_FileSystem_FAT_BootSector_H
#define MathLib_FileSystem_FAT_BootSector_H
#include "FATExtendedBootRecord32.hpp"

namespace MathLib {
    struct FATBootSector {
        static constexpr uint16_t expectedSignature = 0xaa55;
        enum class Type : uint8_t {
            // TODO:
        };
        uint8_t jmp[3];
        char oem[8];
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t reservedSectors;
        uint8_t fatCount;
        uint16_t rootDirectoryEntries;
        uint16_t sectorCount16;
        Type type;
        uint16_t sectorsPerFAT;
        uint16_t sectorsPerTrack;
        uint16_t headCount;
        uint32_t hiddenSectorCount;
        uint32_t sectorCount32;
        union {
            struct {
                FATExtendedBootRecord ebr;
                uint8_t nonFAT32Code[28];
            };
            FATExtendedBootRecord32 ebr32;
        };
        uint8_t code[420];
        uint16_t signature;

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] uint32_t GetSectorCount(void) const;
    } __attribute__((packed));
}

#endif