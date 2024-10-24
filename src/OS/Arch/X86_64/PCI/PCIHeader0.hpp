#ifdef __x86_64__
#ifndef PCIHeader0_H
#define PCIHeader0_H
#include "PCIHeader.hpp"

struct PCIHeader0 : PCIHeader {
    uint32_t bar[6];
    uint32_t cis;
    uint16_t subSystemVendorID;
    uint16_t subSystemID;
    uint32_t expansionROM;
    uint8_t capabilities;
    uint64_t reserved : 56;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint8_t minGrant;
    uint8_t maxLatency;
} __attribute__((packed));

#endif
#endif