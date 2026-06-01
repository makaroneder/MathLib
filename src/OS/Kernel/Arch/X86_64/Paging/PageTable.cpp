#ifdef __x86_64__
#include "PageTable.hpp"

PageTable::PageTable(void) {
    for (uint16_t i = 0; i < 512; i++) entries[i] = PageEntry();
}

#endif