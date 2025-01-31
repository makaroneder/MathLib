#ifdef __x86_64__
#ifndef TSSEntry_H
#define TSSEntry_H
#include "GDTEntry.hpp"

struct TSSEntry : GDTEntry {
    uint32_t base;
    uint32_t reserved;

    uint64_t GetBase(void) const;
    void SetBase(uint64_t base);
} __attribute__((packed));

#endif
#endif