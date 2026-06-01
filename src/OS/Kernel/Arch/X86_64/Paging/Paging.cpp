#ifdef __x86_64__
#include "Paging.hpp"
#include "../CPUID.hpp"
#include <Memory.hpp>

uint8_t firstIndex = 1;
uint8_t lastIndex = 4;
const uint8_t offsets[] = {
    48, 39, 30, 21, 12,
};
void PreparePaging(void) {
    uintptr_t maxCPUID;
    CPUID(0x00, &maxCPUID, nullptr, nullptr, nullptr);
    bool page2MB = false;
    bool page1GB = false;
    if (maxCPUID >= 0x01) {
        uintptr_t d;
        CPUID(0x01, nullptr, nullptr, nullptr, &d);
        if (d & (1 << 3)) page2MB = true;
    }
    CPUID(0x80000000, &maxCPUID, nullptr, nullptr, nullptr);
    if (maxCPUID >= 0x80000001) {
        uintptr_t d;
        CPUID(0x80000001, nullptr, nullptr, nullptr, &d);
        if (d & (1 << 26)) page1GB = true;
    }
    if (page1GB) lastIndex -= 2;
    else if (page2MB) lastIndex--;
}
size_t GetPageSize(void) {
    switch (lastIndex) {
        case 4: return 4 * 1024;
        case 3: return 2 * 1024 * 1024;
        case 2: return 1 * 1024 * 1024 * 1024;
        default: return 0;
    }
}
size_t GetRequiredMemorySizeToMap(uintptr_t address) {
    const size_t pageSize = 1 << offsets[lastIndex];
    const size_t entriesPerTable = 512;
    size_t count = (address + pageSize - 1) / pageSize;
    size_t ret = 0;
    for (uint8_t level = lastIndex; level >= firstIndex; level--) {
        count = (count + entriesPerTable - 1) / entriesPerTable;
        ret += count;
    }
    return ret * 4096;
}
bool Map(MathLib::Allocator& allocator, PageTable* pml4, uintptr_t address) {
    PageTable* prev = pml4;
    for (uint8_t i = firstIndex; i < lastIndex; i++) {
        const uint16_t index = (address >> offsets[i]) & 0x1ff;
        if (prev->entries[index].present) {
            prev = (PageTable*)(prev->entries[index].address << 12);
            continue;
        }
        PageTable* curr = allocator.AllocT<PageTable>();
        if (!curr) return false;
        prev->entries[index] = PageEntry((uintptr_t)curr, true, true, false);
        prev = curr;
    }
    prev->entries[(address >> offsets[lastIndex]) & 0x1ff] = PageEntry(address, true, true, lastIndex + 1 != sizeof(offsets));
    return true;
}

#endif