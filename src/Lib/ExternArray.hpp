#ifndef MathLib_ExternArray_H
#define MathLib_ExternArray_H
#include "Interfaces/Sequence/Collection.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const char* str);
    template <typename T>
    struct ExternArray : Collection<T> {
        ExternArray(T* buffer, size_t size) : buffer(buffer), size(size) {
            if (!buffer && size) Panic("Invalid buffer provided");
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return buffer;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return buffer;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return false;
        }

        private:
        T* buffer;
        size_t size;
    };
}

#endif