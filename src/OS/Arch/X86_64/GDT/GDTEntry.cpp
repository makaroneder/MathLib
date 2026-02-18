#ifdef __x86_64__
#include "GDTEntry.hpp"

uint32_t GDTEntry::GetBase(void) const {
    return (base2 << 24) | base1;
}
void GDTEntry::SetBase(uint32_t base) {
    base1 = base;
    base2 = base >> 24;
}
bool GDTEntry::SetLimit(uint32_t limit) {
    uint32_t mask = 0;
    for (uint8_t i = 20; i < 32; i++) mask |= (1 << i);
    if (limit & mask) return false;
    limit1 = limit;
    limit2 = limit >> 16;
    return true;
}

#endif