#ifdef __x86_64__
#ifndef GDTEntry_H
#define GDTEntry_H
#include <stdint.h>

struct GDTEntry {
    uint16_t limit1;
    uint32_t base1 : 24;
    bool accessed : 1;
    bool readWrite : 1;
    bool directionOrConformingBit : 1;
    bool executable : 1;
    bool type : 1;
    uint8_t ring : 2;
    bool present : 1;
    uint8_t limit2 : 4;
    uint8_t reserved : 1;
    bool longModeCode : 1;
    bool segment32 : 1;
    bool pageGranularity : 1;
    uint8_t base2;

    void SetBase(uint32_t base);
    bool SetLimit(uint32_t limit);
} __attribute__((packed));

#endif
#endif