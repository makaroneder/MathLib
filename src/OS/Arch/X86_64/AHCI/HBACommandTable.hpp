#ifndef HBACommandTable_H
#define HBACommandTable_H
#include <stdint.h>

struct HBAPRDTEntry {
    uint32_t dataAddress;
    uint32_t dataAddressUpper;
    uint32_t reserved1;
    uint32_t count : 22;
    uint16_t reserved2 : 9;
    bool interruptOnCompletion : 1;
};
struct HBACommandTable {
    uint8_t commandFis[64];
    uint8_t atapiCommand[16];
    uint8_t rsv[48];
    HBAPRDTEntry entry[];
};

#endif