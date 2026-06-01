#ifdef __x86_64__
#ifndef PageTable_H
#define PageTable_H
#include "PageEntry.hpp"

struct PageTable {
    PageEntry entries[512];

    PageTable(void);
} __attribute__((packed));

#endif
#endif