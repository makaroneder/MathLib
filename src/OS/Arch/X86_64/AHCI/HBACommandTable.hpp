#ifdef __x86_64__
#ifndef HBACommandTable_H
#define HBACommandTable_H
#include "HBAPRDTEntry.hpp"

struct HBACommandTable {
    uint8_t commandFis[64];
    uint8_t atapiCommand[16];
    uint8_t reserved[48];
    HBAPRDTEntry entry[];
};

#endif
#endif