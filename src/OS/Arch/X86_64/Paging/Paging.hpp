#ifdef __x86_64__
#ifndef Paging_H
#define Paging_H
#include "PageTable.hpp"

bool Map(PageTable* pml4, uintptr_t physicalAddress, uintptr_t virtualAddress);

#endif
#endif