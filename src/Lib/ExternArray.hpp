#ifndef MathLib_ExternArray_H
#define MathLib_ExternArray_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct ExternArray : Collection<T> {
        ExternArray(T* buffer, size_t size) : buffer(buffer), size(size) {
            if (!buffer) Panic("Invalid buffer provided");
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            if (index >= size) Panic("Index out of bounds");
            return buffer[index];
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            if (index >= size) Panic("Index out of bounds");
            return buffer[index];
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return buffer;
        }

        private:
        T* buffer;
        size_t size;
    };
}

#endif