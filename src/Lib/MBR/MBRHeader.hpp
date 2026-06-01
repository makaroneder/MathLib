#ifndef MathLib_MBR_Header_H
#define MathLib_MBR_Header_H
#include "MBRPartitionEntry.hpp"

namespace MathLib {
    struct MBRHeader {
        static constexpr uint16_t expectedSignature = 0xaa55;
        enum class Attributes : uint16_t {
            Normal = 0,
            ReadOnly = 0x5a5a,
        };
        uint8_t code[440];
        uint32_t uniqueDiskID;
        Attributes attributes;
        MBRPartitionEntry entries[4];
        uint16_t signature;

        MBRHeader(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif