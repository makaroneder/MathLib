#ifdef __x86_64__
#include "TSSEntry.hpp"

void TSSEntry::SetBase(uint64_t base_) {
    GDTEntry::SetBase(base_);
    base = base_ >> 32;
}

#endif