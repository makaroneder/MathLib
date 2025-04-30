#ifndef ExtendedTARHeader_H
#define ExtendedTARHeader_H
#include "TARHeader.hpp"

struct ExtendedTARHeader : TARHeader {
    char signature[6];
    char version[2];
    char ownerName[32];
    char groupName[32];
    char deviceMajor[8];
    char deviceMinor[8];

    [[nodiscard]] bool IsValid(const char* expectedSignature, const char* expectedVersion) const;
    [[nodiscard]] bool Create(const MathLib::Sequence<char>& path, size_t dataSize, const char* expectedSignature, const char* expectedVersion);
} __attribute__((packed));

#endif