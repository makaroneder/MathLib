#ifndef MathLib_Interfaces_Collection_H
#define MathLib_Interfaces_Collection_H
#include "NonLinearCollection.hpp"
#include "../Iteratable.hpp"
#include "../Memory.hpp"

namespace MathLib {
    template <typename T>
    struct Collection : NonLinearCollection<T>, Iteratable<T> {
        [[nodiscard]] virtual T* GetValue(void) = 0;
        [[nodiscard]] virtual const T* GetValue(void) const = 0;
        [[nodiscard]] virtual T At(size_t index) const override {
            StartBenchmark
            if (index >= this->GetSize()) Panic("Index out of bounds");
            const T* buffer = GetValue();
            if (!buffer) Panic("Buffer not allocated");
            ReturnFromBenchmark(buffer[index]);
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            StartBenchmark
            if (index >= this->GetSize()) Panic("Index out of bounds");
            T* buffer = GetValue();
            if (!buffer) Panic("Buffer not allocated");
            ReturnFromBenchmark(buffer[index]);
        }
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            StartAndReturnFromBenchmark(Iterator<const T>(GetValue()));
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            StartAndReturnFromBenchmark(Iterator<const T>(GetValue() + this->GetSize()));
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            StartAndReturnFromBenchmark(Iterator<T>(GetValue()));
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            StartAndReturnFromBenchmark(Iterator<T>(GetValue() + this->GetSize()));
        }
        [[nodiscard]] bool Copy(Collection<T>& collection, size_t start, size_t n) const {
            StartBenchmark
            if (start + n >= this->GetSize()) ReturnFromBenchmark(false);
            const size_t size = collection.GetSize();
            if (n >= size) ReturnFromBenchmark(false);
            MemoryCopy(GetValue() + n, collection.GetValue(), size);
            ReturnFromBenchmark(true);
        }
    };
}

#endif