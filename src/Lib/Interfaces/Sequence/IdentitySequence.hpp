#ifndef MathLib_Interfaces_Sequence_Identity_H
#define MathLib_Interfaces_Sequence_Identity_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct IdentitySequence : Sequence<T> {
        IdentitySequence(size_t size) : size(size) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return (T)index;
        }
    
        private:
        size_t size;
    };
}

#endif