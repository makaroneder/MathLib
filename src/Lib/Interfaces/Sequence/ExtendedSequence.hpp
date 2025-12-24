#ifndef MathLib_Interfaces_Sequence_Extended_H
#define MathLib_Interfaces_Sequence_Extended_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct ExtendedSequence : Sequence<T> {
        ExtendedSequence(const Sequence<T>& sequence, const T& element, size_t size) : element(element), size(size), sequence(sequence) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize() + size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return index < sequence.GetSize() ? sequence.AtUnsafe(index) : element;
        }

        private:
        T element;
        size_t size;
        const Sequence<T>& sequence;
    };
}

#endif