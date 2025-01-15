#ifdef __x86_64__
#include "PageTable.hpp"

void PageTable::MakeUnused(void) {
    for (size_t i = 0; i < SizeOfArray(entries); i++) entries[i].MakeUnused();
}
MathLib::Expected<size_t> PageTable::GetNextTable(size_t i) {
    return (entries[i].present && !entries[i].huge) ? MathLib::Expected<size_t>(((uintptr_t)this << 9) | (i << 12)) : MathLib::Expected<size_t>();
}

#endif