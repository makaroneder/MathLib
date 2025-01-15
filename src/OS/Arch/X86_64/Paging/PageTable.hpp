#ifdef __x86_64__
#ifndef PageTable_H
#define PageTable_H
#include "PageEntry.hpp"

struct PageTable {
    PageEntry entries[512];

    void MakeUnused(void);
    [[nodiscard]] MathLib::Expected<size_t> GetNextTable(size_t i);
} __attribute__((packed));

#endif
#endif