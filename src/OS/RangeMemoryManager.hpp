#ifndef RangeMemoryManager_H
#define RangeMemoryManager_H
#include "MemoryManager.hpp"
#include <Interval.hpp>

struct RangeMemoryManager : MemoryManager {
    bool AddRegion(Interval<uintptr_t> region);
    virtual void* At(size_t pos, size_t size) override;
    virtual size_t GetSize(void) const override;

    private:
    Array<Interval<uintptr_t>> regions;
};

#endif