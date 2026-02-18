#ifdef __x86_64__
#ifndef PCIHeader1_H
#define PCIHeader1_H
#include "PCIHeader.hpp"

struct PCIHeader1 : PCIHeader {
    uint32_t bar[2];
    uint8_t primaryBus;
    uint8_t secondaryBus;
    uint8_t subordinateBus;
    uint8_t secondaryLatencyTimer;
    uint8_t ioBase;
    uint8_t ioLimit;
    uint16_t secondaryStatus;
    uint16_t memoryBase;
    uint16_t memoryLimit;
    uint16_t prefetchableMemoryBase;
    uint16_t prefetchableMemoryLimit;
    uint32_t prefetchableMemoryBaseUpper32Bits;
    uint32_t prefetchableMemoryLimitUpper32Bits;
    uint16_t ioBaseUpper16Bits;
    uint16_t ioLimitUpper16Bits;
    uint8_t capabilities;
    uint32_t reserved : 24;
    uint32_t expansionROM;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint16_t bridgeControl;
} __attribute__((packed));

#endif
#endif