#ifndef MathLib_Interfaces_Collection_H
#define MathLib_Interfaces_Collection_H
#include "NonLinearCollection.hpp"
#include "../Iteratable.hpp"
#include "../Memory.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : NonLinearCollection<T>, Iteratable<T> {
        [[nodiscard]] virtual const T* GetValue(void) const = 0;
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            return Iterator<const T>(GetValue());
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            return Iterator<const T>(GetValue() + this->GetSize());
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            return Iterator<T>((T*)GetValue());
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            return Iterator<T>((T*)GetValue() + this->GetSize());
        }
        [[nodiscard]] bool Copy(Collection<T>& collection, size_t start, size_t n) const {
            if (start + n >= this->GetSize()) return false;
            const size_t size = collection.GetSize();
            if (n >= size) return false;
            MemoryCopy(GetValue() + n, collection.GetValue(), size);
            return true;
        }
    };
}

#endif