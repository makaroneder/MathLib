#include "LinearAllocator.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    LinearAllocator::LinearAllocator(void* buffer, size_t size) : BufferAllocator(buffer, size), offset(0) {}
    LinearAllocator::LinearAllocator(Allocator* allocator, size_t size) : BufferAllocator(allocator, size), offset(0) {}
    bool LinearAllocator::Save(Writable& file) const {
        return BufferAllocator::Save(file) && file.Write<size_t>(offset);
    }
    bool LinearAllocator::Load(Readable& file) {
        return BufferAllocator::Load(file) && file.Read<size_t>(offset);
    }
    void* LinearAllocator::Alloc(size_t size) {
        if (offset + size >= this->size) return nullptr;
        uint8_t* const ret = (uint8_t*)buffer + offset;
        offset += size;
        return ret;
    }
    bool LinearAllocator::Dealloc(void* ptr) {
        return IsBetween((uintptr_t)ptr, (uintptr_t)buffer, (uintptr_t)buffer + size);
    }
    size_t LinearAllocator::GetFreeMemory(void) const {
        return size - offset;
    }
}