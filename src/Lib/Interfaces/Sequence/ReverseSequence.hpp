#ifndef MathLib_Interfaces_Sequence_Reverse_H
#define MathLib_Interfaces_Sequence_Reverse_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct ReverseSequence : Sequence<T> {
        ReverseSequence(const Sequence<T>& sequence) : sequence(sequence) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(sequence.GetSize() - index - 1);
        }

        private:
        const Sequence<T>& sequence;
    };
}

#endif