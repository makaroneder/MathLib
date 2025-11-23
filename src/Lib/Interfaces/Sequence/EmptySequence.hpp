#ifndef MathLib_Interfaces_Sequence_Empty_H
#define MathLib_Interfaces_Sequence_Empty_H
#include "Collection.hpp"

namespace MathLib {
    template <typename T>
    struct EmptySequence : Collection<T> {
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return 0;
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return true;
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return nullptr;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return nullptr;
        }
    };
}

#endif