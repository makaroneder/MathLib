#ifndef MathLib_Interfaces_Sequence_Wrap_H
#define MathLib_Interfaces_Sequence_Wrap_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct WrapSequence : Sequence<T> {
        WrapSequence(const Sequence<T>& sequence, size_t size) : sequence(sequence), size(size) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(index % sequence.GetSize());
        }

        private:
        const Sequence<T>& sequence;
        size_t size;
    };
}

#endif