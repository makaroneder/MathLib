#ifndef UnixStandardTARHeader_H
#define UnixStandardTARHeader_H
#include "../ExtendedTARHeader.hpp"

struct UnixStandardTARHeader : ExtendedTARHeader {
    static constexpr const char* expectedSignature = "ustar";
    static constexpr const char* expectedVersion = "00";
    char pathPrefix[155];
    uint32_t reserved[3];

    [[nodiscard]] bool IsValid(void) const;
    [[nodiscard]] MathLib::String GetName(void) const;
    [[nodiscard]] bool Create(const MathLib::Sequence<char>& path, size_t dataSize);
} __attribute__((packed));

#endif