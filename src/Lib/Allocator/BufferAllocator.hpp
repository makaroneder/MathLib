#ifndef MathLib_Allocator_Buffer_H
#define MathLib_Allocator_Buffer_H
#include "Allocator.hpp"
#include "../Interfaces/Saveable.hpp"

namespace MathLib {
    struct BufferAllocator : Allocator, Saveable, ByteDevice {
        BufferAllocator(void* buffer, size_t size);
        BufferAllocator(Allocator* allocator, size_t size);
        virtual ~BufferAllocator(void) override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const = 0;
        [[nodiscard]] void* GetBuffer(void) const;

        protected:
        void* buffer;
        size_t size;

        private:
        Allocator* allocator;
    };
}

#endif