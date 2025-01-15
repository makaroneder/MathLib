#ifdef __x86_64__
#include "PageEntry.hpp"

void PageEntry::MakeUnused(void) {
    *(uint64_t*)this = 0;
}
MathLib::Expected<uintptr_t> PageEntry::GetAddress(void) const {
    return present ? MathLib::Expected<uintptr_t>(address) : MathLib::Expected<uintptr_t>();
}

#endif