#ifndef MathLib_Allocator_Buffer_H
#define MathLib_Allocator_Buffer_H
#include "Allocator.hpp"

namespace MathLib {
    struct BufferAllocator : Allocator {
        BufferAllocator(void* buffer, size_t size);
        BufferAllocator(Allocator* allocator, size_t size);
        virtual ~BufferAllocator(void) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const = 0;
        [[nodiscard]] void* GetBuffer(void) const;
        [[nodiscard]] size_t GetSize(void) const;

        protected:
        void* buffer;
        size_t size;

        private:
        Allocator* allocator;
    };
}

#endif