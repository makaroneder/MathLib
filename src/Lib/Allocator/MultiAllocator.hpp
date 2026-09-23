#ifndef MathLib_Allocator_Multi_H
#define MathLib_Allocator_Multi_H
#include "Allocator.hpp"

namespace MathLib {
    struct MultiAllocator : Allocator {
        MultiAllocator(void);
        MultiAllocator(Allocator* allocator);
        MultiAllocator(Allocator* allocator1, Allocator* allocator2);
        MultiAllocator(MultiAllocator&& other);
        MultiAllocator(const MultiAllocator& other) = delete;
        virtual ~MultiAllocator(void) override;
        MultiAllocator& operator=(const MultiAllocator& other) = delete;
        MultiAllocator& operator=(MultiAllocator&& other);
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] bool AddAllocator(Allocator* allocator);

        private:
        Allocator* allocator1;
        Allocator* allocator2;
    };
}

#endif