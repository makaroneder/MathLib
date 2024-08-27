#include "TSSEntry.hpp"

void TSSEntry::SetBase(uint64_t base_) {
    GDTEntry::SetBase(base_);
    base = base_ >> 32;
}