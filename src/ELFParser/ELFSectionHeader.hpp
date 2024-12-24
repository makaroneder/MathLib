#ifndef ELFSectionHeader_H
#define ELFSectionHeader_H
#include <Typedefs.hpp>

struct ELFSectionHeader {
    uint32_t name;
    uint32_t type; // TODO:
    uint64_t flags; // TODO:
    uint64_t address;
    uint64_t offset;
    uint64_t size;
    uint32_t link;
    uint32_t info;
    uint64_t alignment;
    uint64_t entrySize;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    MathLib::String ToString(const MathLib::String& padding = "") const;
} __attribute__((packed));

#endif