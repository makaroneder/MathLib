#ifdef __x86_64__
#ifndef AlignedPageTable_H
#define AlignedPageTable_H
#include "PageTable.hpp"

struct AlignedPageTable {
    PageTable table;
} __attribute__((packed, aligned(4096)));

#endif
#endif