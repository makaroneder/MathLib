#include "RegionAllocator.hpp"
#include "../Host.hpp"

namespace MathLib {
    RegionAllocator::Region::Region(size_t size, bool free) : size(size), free(free), reserved(0) {
        EmptyBenchmark
    }
    RegionAllocator::RegionAllocator(void* buffer, size_t size) : BufferAllocator(buffer, size) {
        StartBenchmark
        if (size <= sizeof(Region)) Panic("Not enough memory for allocator");
        *(Region*)buffer = Region(size, true);
        EndBenchmark
    }
    RegionAllocator::RegionAllocator(Allocator* allocator, size_t size) : BufferAllocator(allocator, size) {
        StartBenchmark
        if (size <= sizeof(Region)) Panic("Not enough memory for allocator");
        *(Region*)buffer = Region(size, true);
        EndBenchmark
    }
    void* RegionAllocator::Alloc(size_t size) {
        StartBenchmark
        size_t pos = 0;
        Region* ret = nullptr;
        while (pos < this->size) {
            Region* region = (Region*)((uintptr_t)buffer + pos);
            pos += region->size + sizeof(Region);
            if (region->free) {
                if (region->size == size) {
                    ret = region;
                    break;
                }
                else if (region->size > size && (!ret || region->size < ret->size)) ret = region;
            }
        }
        if (!ret) ReturnFromBenchmark(nullptr);
        ret->free = false;
        uint8_t* addr = (uint8_t*)ret + sizeof(Region);
        if (ret->size - size > sizeof(Region)) {
            *(Region*)(addr + size) = Region(ret->size - size - sizeof(Region), true);
            ret->size = size;
        }
        for (size_t i = 0; i < ret->size; i++) addr[i] = 0;
        ReturnFromBenchmark(addr);
    }
    bool RegionAllocator::Dealloc(void* ptr) {
        StartBenchmark
        size_t pos = 0;
        Region* prev = nullptr;
        bool found = false;
        while (pos < size) {
            Region* region = (Region*)((uintptr_t)buffer + pos);
            pos += region->size + sizeof(Region);
            if ((uintptr_t)region == (uintptr_t)ptr - sizeof(Region)) {
                if (region->free) ReturnFromBenchmark(false);
                region->free = true;
                found = true;
            }
            if (region->free) {
                if (prev) prev->size += region->size + sizeof(Region);
                else prev = region;
            }
            else prev = nullptr;
        }
        ReturnFromBenchmark(found);
    }
    size_t RegionAllocator::GetFreeMemory(void) const {
        StartBenchmark
        size_t pos = 0;
        size_t ret = 0;
        while (pos < size) {
            Region* region = (Region*)((uintptr_t)buffer + pos);
            pos += region->size + sizeof(Region);
            ret += region->size * region->free;
        }
        ReturnFromBenchmark(ret);
    }
}