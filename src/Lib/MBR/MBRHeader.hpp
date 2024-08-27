#ifndef MBRHeader_H
#define MBRHeader_H
#include "MBRPartitionEntry.hpp"

/// @brief MBR boot sector
struct MBRHeader {
    /// @brief Expected boot signature
    static constexpr uint16_t expectedSignature = 0xaa55;
    /// @brief Optional attributes
    enum class Attributes : uint16_t {
        Normal = 0,
        ReadOnly = 0x5a5a,
    };
    /// @brief MBR code
    uint8_t code[440];
    /// @brief Optional disk id
    uint32_t uniqueDiskID;
    /// @brief Optional attributes
    Attributes attributes;
    /// @brief Primary partitions
    MBRPartitionEntry entries[4];
    /// @brief Boot signature
    uint16_t signature;

    /// @brief Creates default MBR boot sector
    MBRHeader(void);
    /// @brief Checks whether MBR boot sector is valid
    /// @return Validity of MBR boot sector
    bool IsValid(void) const;
} __attribute__((packed));

#endif