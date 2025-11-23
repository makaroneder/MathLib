#ifndef MathLib_Allocator_Multi_H
#define MathLib_Allocator_Multi_H
#include "Allocator.hpp"

namespace MathLib {
    struct MultiAllocator : Allocator {
        MultiAllocator(void);
        MultiAllocator(Allocator* allocator);
        MultiAllocator(Allocator* allocator1, Allocator* allocator2);
        virtual ~MultiAllocator(void) override;
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
        [[nodiscard]] bool AddAllocator(Allocator* allocator);

        private:
        Allocator* allocator1;
        Allocator* allocator2;
    };
}

#endif