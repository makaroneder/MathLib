#ifndef MathLib_Interfaces_Sequence_Collection_H
#define MathLib_Interfaces_Sequence_Collection_H
#include "NonLinearCollection.hpp"
#include "../../Iteratable.hpp"
#include "../../Memory.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : NonLinearCollection<T>, Iteratable<T> {
        using Sequence<T>::At;
        using NonLinearCollection<T>::At;
        [[nodiscard]] virtual T* GetValue(void) = 0;
        [[nodiscard]] virtual const T* GetValue(void) const = 0;
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            const T* buffer = GetValue();
            if (buffer) return buffer[index];
            Panic("Buffer not allocated");
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            T* buffer = GetValue();
            if (buffer) return buffer[index];
            Panic("Buffer not allocated");
        }
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            return Iterator<const T>(GetValue());
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            return Iterator<const T>(GetValue() + this->GetSize());
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            return Iterator<T>(GetValue());
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            return Iterator<T>(GetValue() + this->GetSize());
        }
        void CopyFromBuffer(const T* buffer) {
            MemoryCopy(buffer, GetValue(), this->GetSize() * sizeof(T));
        }
        [[nodiscard]] bool CopyFromBuffer(const Collection<T>& buffer) {
            const size_t size = this->GetSize();
            if (size != buffer.GetSize()) return false;
            MemoryCopy(buffer.GetValue(), GetValue(), size * sizeof(T));
            return true;
        }
        #define MakeFillBytes(n)                                                                            \
        void FillBytes##n(uint##n##_t value) {                                                              \
            MemorySet<uint##n##_t>(GetValue(), this->GetSize() * sizeof(T) / sizeof(uint##n##_t), value);   \
        }
        MakeFillBytes(8)
        MakeFillBytes(16)
        MakeFillBytes(32)
        MakeFillBytes(64)
        #undef MakeFillBytes
    };
}

#endif