#ifndef MathLib_Allocator_Linear_H
#define MathLib_Allocator_Linear_H
#include "BufferAllocator.hpp"

namespace MathLib {
    struct LinearAllocator : BufferAllocator {
        LinearAllocator(void* buffer, size_t size);
        LinearAllocator(Allocator* allocator, size_t size);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const override;

        private:
        size_t offset;
    };
}

#endif