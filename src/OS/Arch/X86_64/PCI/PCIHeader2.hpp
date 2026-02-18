#ifdef __x86_64__
#ifndef PCIHeader2_H
#define PCIHeader2_H
#include "PCIHeader.hpp"

struct PCIHeader2 : PCIHeader {
    uint32_t cardBus;
    uint8_t capabilitiesOffset;
    uint8_t reserved;
    uint16_t secondaryStatus;
    uint8_t pciBus;
    uint8_t cardBus;
    uint8_t subordinateBus;
    uint8_t cardBusLatencyTimer;
    uint32_t memoryBase0;
    uint32_t memoryLimit0;
    uint32_t memoryBase1;
    uint32_t memoryLimit1;
    uint32_t ioBase0;
    uint32_t ioLimit0;
    uint32_t ioBase1;
    uint32_t ioLimit1;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint16_t bridgeControl;
    uint16_t subSystemDeviceID;
    uint16_t subSystemVendorID;
    uint32_t legacyPCCard;
} __attribute__((packed));

#endif
#endif