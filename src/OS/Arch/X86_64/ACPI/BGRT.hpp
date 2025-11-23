#ifdef __x86_64__
#ifndef BGRT_H
#define BGRT_H
#include <ACPI/ACPITable.hpp>

struct BGRT : MathLib::ACPITable {
    static constexpr const char* expectedSignature = "BGRT";
    static constexpr const uint16_t expectedVersion = 1;
    enum class Type : uint8_t {
        Bitmap = 0,
    };

    uint16_t version;
    uint8_t reserved : 5;
    uint8_t orientation : 2;
    bool displayed : 1;
    Type type;
    uint64_t address;
    uint32_t x;
    uint32_t y;

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));

#endif
#endif