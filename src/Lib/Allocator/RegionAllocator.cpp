#include "RegionAllocator.hpp"
#include "../Host.hpp"

namespace MathLib {
    RegionAllocator::Region::Region(size_t size, bool free) : size(size), free(free), reserved(0) {}
    RegionAllocator::RegionAllocator(void* buffer, size_t size) : BufferAllocator(buffer, size) {
        if (size <= sizeof(Region)) Panic("Not enough memory for allocator");
        *(Region*)buffer = Region(size, true);
    }
    RegionAllocator::RegionAllocator(Allocator* allocator, size_t size) : BufferAllocator(allocator, size) {
        if (size <= sizeof(Region)) Panic("Not enough memory for allocator");
        *(Region*)buffer = Region(size, true);
    }
    void* RegionAllocator::Alloc(size_t size) {
        size_t pos = 0;
        Region* ret = nullptr;
        while (pos < this->size) {
            Region* const region = (Region*)((uintptr_t)buffer + pos);
            pos += region->size + sizeof(Region);
            if (region->free) {
                if (region->size == size) {
                    ret = region;
                    break;
                }
                if (region->size > size && (!ret || region->size < ret->size)) ret = region;
            }
        }
        if (!ret) return nullptr;
        ret->free = false;
        uint8_t* const addr = (uint8_t*)ret + sizeof(Region);
        if (ret->size - size > sizeof(Region)) {
            *(Region*)(addr + size) = Region(ret->size - size - sizeof(Region), true);
            ret->size = size;
        }
        for (size_t i = 0; i < ret->size; i++) addr[i] = 0;
        return addr;
    }
    bool RegionAllocator::Dealloc(void* ptr) {
        if (!ptr) return true;
        size_t pos = 0;
        Region* prev = nullptr;
        bool found = false;
        while (pos < size) {
            Region* const region = (Region*)((uintptr_t)buffer + pos);
            pos += region->size + sizeof(Region);
            if ((uintptr_t)region == (uintptr_t)ptr - sizeof(Region)) {
                if (region->free) return false;
                region->free = true;
                found = true;
            }
            if (region->free) {
                if (prev) prev->size += region->size + sizeof(Region);
                else prev = region;
            }
            else prev = nullptr;
        }
        return found;
    }
    size_t RegionAllocator::GetFreeMemory(void) const {
        uintptr_t pos = (uintptr_t)buffer;
        size_t ret = 0;
        while (pos < size) {
            const Region* const region = (const Region*)pos;
            pos += region->size + sizeof(Region);
            ret += region->size * region->free;
        }
        return ret;
    }
}