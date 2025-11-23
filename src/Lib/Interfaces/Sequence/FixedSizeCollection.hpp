#ifndef MathLib_Interfaces_Sequence_FixedSizeCollection_H
#define MathLib_Interfaces_Sequence_FixedSizeCollection_H
#include "Collection.hpp"

namespace MathLib {
    template <typename T, size_t size>
    struct FixedSizeCollection : Collection<T> {
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return false;
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return buffer;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return buffer;
        }

        private:
        T buffer[size];
    };
}

#endif