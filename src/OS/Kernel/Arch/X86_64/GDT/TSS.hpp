#ifdef __x86_64__
#ifndef TSS_H
#define TSS_H
#include <stdint.h>

struct TSS {
    uint32_t reserved1;
    uint64_t sp[3];
    uint64_t reserved2;
    uint64_t interruptStackTable[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t ioMap;
} __attribute__((packed));

void InitTSS(void);

#endif
#endif