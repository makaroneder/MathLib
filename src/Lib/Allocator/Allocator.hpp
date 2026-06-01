#ifndef MathLib_Allocator_H
#define MathLib_Allocator_H
#include "Allocatable.hpp"

namespace MathLib {
    struct Allocator : Allocatable {
        virtual ~Allocator(void) override;
        [[nodiscard]] virtual void* Alloc(size_t size) = 0;
        [[nodiscard]] virtual bool Dealloc(void* ptr) = 0;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const = 0;
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        template <typename T>
        [[nodiscard]] T* AllocT(void) {
            T* ret = (T*)Alloc(sizeof(T));
            if (ret) *ret = T();
            return ret;
        }
    };
    extern Allocator* allocator;
}

#endif