#ifndef GNUTARHeader_H
#define GNUTARHeader_H
#include "GNUTARSparseFileData.hpp"
#include "../ExtendedTARHeader.hpp"

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
    [[nodiscard]] bool Create(const MathLib::Sequence<char>& path, size_t dataSize);
} __attribute__((packed));

#endif