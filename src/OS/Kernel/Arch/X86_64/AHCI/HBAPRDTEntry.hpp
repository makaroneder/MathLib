#ifdef __x86_64__
#ifndef HBAPRDTEntry_H
#define HBAPRDTEntry_H
#include <stdint.h>

struct HBAPRDTEntry {
    uint64_t dataAddress;
    uint32_t reserved1;
    uint32_t count : 22;
    uint16_t reserved2 : 9;
    bool interruptOnCompletion : 1;
};

#endif
#endif