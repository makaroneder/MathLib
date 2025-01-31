#ifdef __x86_64__
#include "TSSEntry.hpp"

uint64_t TSSEntry::GetBase(void) const {
    return GDTEntry::GetBase() | ((uint64_t)base << 32);
}
void TSSEntry::SetBase(uint64_t base_) {
    GDTEntry::SetBase(base_);
    base = base_ >> 32;
}

#endif