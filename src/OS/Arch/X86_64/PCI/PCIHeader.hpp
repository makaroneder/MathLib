#ifdef __x86_64__
#ifndef PCIHeader_H
#define PCIHeader_H
#include <stdint.h>

struct PCIHeader {
    uint16_t vendorID;
    uint16_t deviceID;
    uint16_t command;
    uint16_t status;
    uint8_t revisionID;
    uint8_t programInterface;
    uint8_t subClass;
    uint8_t classCode;
    uint8_t cacheLineSize;
    uint8_t latencyTimer;
    uint8_t type : 7;
    bool multipleFunctions : 1;
    uint8_t completionCode : 3;
    uint8_t reserved : 2;
    uint8_t startBIST : 1;
    bool bistCapable : 1;

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));

#endif
#endif