#ifdef __x86_64__
#include "Paging.hpp"
#include <Memory.hpp>

bool Map(PageTable* pml4, uintptr_t physicalAddress, uintptr_t virtualAddress) {
    uintptr_t tmp = virtualAddress >> 12;
    PageTable* prev = pml4;
    for (uint8_t i = 0; i < 3; i++) {
        const uint16_t index = tmp & 0x1ff;
        tmp >>= 9;
        if (prev->entries[index].present) {
            prev = (PageTable*)(prev->entries[index].address << 12);
            continue;
        }
        PageTable* curr = new PageTable();
        if (!curr) return false;
        prev->entries[index] = PageEntry((uintptr_t)curr, true, true);
        prev = curr;
    }
    prev->entries[tmp & 0x1ff] = PageEntry(physicalAddress, true, true);
    return true;
}

#endif