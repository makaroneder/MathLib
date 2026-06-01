#ifndef MathLib_Interfaces_Sequence_Constant_H
#define MathLib_Interfaces_Sequence_Constant_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct ConstantSequence : Sequence<T> {
        ConstantSequence(const T& element, size_t size) : element(element), size(size) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t) const override {
            return element;
        }

        private:
        T element;
        size_t size;
    };
}

#endif