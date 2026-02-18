#ifndef MathLib_FileSystem_FAT_FSInfo_H
#define MathLib_FileSystem_FAT_FSInfo_H
#include "FATExtendedBootRecord32.hpp"

namespace MathLib {
    struct FATFSInfo {
        static constexpr const char* expectedSignature1 = "RRaA";
        static constexpr const char* expectedSignature2 = "rrAa";
        static constexpr uint32_t expectedSignature3 = 0xaa550000;
        char signature1[4];
        uint64_t reserved1[60];
        char signature2[4];
        uint32_t lastFreeCluster;
        uint32_t firstAvailableCluster;
        uint32_t reserved2[3];
        uint32_t signature3;

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool operator==(const FATFSInfo& other) const;
        [[nodiscard]] bool operator!=(const FATFSInfo& other) const;
    } __attribute__((packed));
}

#endif