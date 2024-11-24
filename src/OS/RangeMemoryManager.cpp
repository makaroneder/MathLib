#include "RangeMemoryManager.hpp"

extern "C" uintptr_t start;
extern "C" uintptr_t end;
bool RangeMemoryManager::AddRegion(MathLib::Interval<uintptr_t> region) {
    if (!region.GetSize()) return false;
    if (!region.GetMin()) {
        if (region.GetSize() > 1) region = MathLib::Interval<uintptr_t>(region.GetMin() + 1, region.GetMax());
        else return false;
    }
    return regions.Add(region);
}
void* RangeMemoryManager::At(size_t pos, size_t size) {
    const MathLib::Interval<uintptr_t> kernelInterval = MathLib::Interval<uintptr_t>((uintptr_t)&start, (uintptr_t)&end);
    for (const MathLib::Interval<uintptr_t>& region : regions) {
        const size_t prev = pos;
        while (pos + size < region.GetSize() && kernelInterval.Contains(region.GetMin() + pos + size)) pos++;
        if (pos + size < region.GetSize()) return (void*)(region.GetMin() + pos);
        else pos = prev - region.GetSize();
    }
    return nullptr;
}
size_t RangeMemoryManager::GetSize(void) const {
    size_t size = 0;
    for (const MathLib::Interval<uintptr_t>& region : regions) size += region.GetSize();
    return size;
}