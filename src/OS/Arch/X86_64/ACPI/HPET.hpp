#ifndef HPET_H
#define HPET_H
#include "ACPITable.hpp"
#include "GenericAddressStructure.hpp"

struct HPET : ACPITable {
    static constexpr const char* expectedSignature = "HPET";
    uint8_t hardwareRevision;
    uint8_t comparatorCount : 5;
    uint8_t counterSize : 1;
    uint8_t reserved : 1;
    bool legacyIRQReplacement : 1;
    uint16_t pciVendor;
    GenericAddressStructure address;
    uint8_t number;
    uint16_t minimumTick;
    uint8_t pageProtection;
} __attribute__((packed));

#endif