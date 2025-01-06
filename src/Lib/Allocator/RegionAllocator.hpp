#ifndef MathLib_Allocator_Region_H
#define MathLib_Allocator_Region_H
#include "BufferAllocator.hpp"
#include <stdint.h>

namespace MathLib {
    struct RegionAllocator : BufferAllocator {
        struct Region {
            size_t size;
            bool free : 1;
            uint8_t reserved : 7;

            Region(size_t size, bool free);
        } __attribute__((packed));
        RegionAllocator(void* buffer, size_t size);
        RegionAllocator(Allocator* allocator, size_t size);
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const override;
    };
}

#endif