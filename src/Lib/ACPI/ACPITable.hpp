#ifndef MathLib_ACPI_Table_H
#define MathLib_ACPI_Table_H
#include <stdint.h>

namespace MathLib {
    struct ACPITable {
        char signature[4];
        uint32_t length;
        uint8_t revision;
        uint8_t checksum;
        char oemID[6];
        char oemTableID[8];
        uint32_t oemRevision;
        uint32_t creatorID;
        uint32_t creatorRevision;

        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif