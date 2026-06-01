#ifdef __x86_64__
#ifndef HBACommandHeader_H
#define HBACommandHeader_H
#include <stdint.h>

struct HBACommandHeader {
    uint8_t cmdFisLen : 5;
    bool atapi : 1;
    bool write : 1;
    bool prefetchable : 1;
    bool reset : 1;
    bool bist : 1;
    bool clearBusy : 1;
    uint8_t reserved1 : 1;
    uint8_t portMultiplier : 4;
    uint16_t prdtLen;
    uint32_t prdbCount;
    uint64_t cmdTableBaseAddress;
    uint64_t reserved2[2];
};

#endif
#endif