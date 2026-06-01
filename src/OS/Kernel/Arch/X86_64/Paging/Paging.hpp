#ifdef __x86_64__
#ifndef Paging_H
#define Paging_H
#include "PageTable.hpp"
#include <Allocator/Allocator.hpp>

void PreparePaging(void);
[[nodiscard]] size_t GetPageSize(void);
[[nodiscard]] size_t GetRequiredMemorySizeToMap(uintptr_t address);
[[nodiscard]] bool Map(MathLib::Allocator& allocator, PageTable* pml4, uintptr_t address);

#endif
#endif