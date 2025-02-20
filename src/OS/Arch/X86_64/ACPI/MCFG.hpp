#ifdef __x86_64__
#ifndef MCFG_H
#define MCFG_H
#include <ACPI/ACPITable.hpp>

struct MCFGEntry {
    uint64_t address;
    uint16_t pciSegment;
    uint8_t startBus;
    uint8_t endBus;
    uint32_t reserved;
} __attribute__((packed));
struct MCFG : MathLib::ACPITable {
    static constexpr const char* expectedSignature = "MCFG";
    uint64_t reserved;
    MCFGEntry entries[];
} __attribute__((packed));

#endif
#endif