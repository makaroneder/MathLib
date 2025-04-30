#ifndef MathLib_ExternArray_H
#define MathLib_ExternArray_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct ExternArray : Collection<T> {
        ExternArray(T* buffer, size_t size) : buffer(buffer), size(size) {
            StartBenchmark
            if (!buffer) Panic("Invalid buffer provided");
            EndBenchmark
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartBenchmark
            if (index >= size) Panic("Index out of bounds");
            ReturnFromBenchmark(buffer[index]);
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            StartBenchmark
            if (index >= size) Panic("Index out of bounds");
            ReturnFromBenchmark(buffer[index]);
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            StartAndReturnFromBenchmark(false);
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            StartAndReturnFromBenchmark(buffer);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartAndReturnFromBenchmark(false);
        }

        private:
        T* buffer;
        size_t size;
    };
}

#endif