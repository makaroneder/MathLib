#ifdef __x86_64__
#ifndef Multiboot_H
#define Multiboot_H
#include <Typedefs.hpp>

struct MultibootAPM {
    uint16_t version;
    uint16_t codeSegment;
    uint32_t offset;
    uint16_t codeSegment16;
    uint16_t dataSegment;
    uint16_t flags;
    uint16_t codeSegmentLength;
    uint16_t codeSegment16Length;
    uint16_t dataSegmentLength;

    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] MathLib::String ToString(const MathLib::String& padding = "") const;
};
struct MultibootFramebuffer {
    enum class Type : uint8_t {
        Indexed = 0,
        RGB,
        EGA,
        TypeCount,
    };
    static constexpr const char* typeStr[] = {
        "Indexed",
        "RGB",
        "EGA",
    };
    uint64_t address;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bitsPerPixel;
    Type type;

    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] MathLib::String ToString(const MathLib::String& padding = "") const;
} __attribute__((packed));
enum class MultibootMemoryMapEntryType : uint32_t {
    Available = 1,
    Reserved,
    ACPIReclaimable,
    NVS,
    BadRAM,
    TypeCount,
};
static constexpr const char* multibootMemoryMapEntryTypeStr[] = {
    "Available",
    "Reserved",
    "ACPI reclaimable",
    "NVS",
    "Bad RAM",
};

#endif
#endif