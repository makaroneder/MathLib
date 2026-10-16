#ifndef MathLib_Interfaces_Sequence_Convert_H
#define MathLib_Interfaces_Sequence_Convert_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T, typename F>
    struct ConvertSequence : Sequence<T> {
        ConvertSequence(const Sequence<F>& sequence) : sequence(sequence) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return T(sequence.AtUnsafe(index));
        }

        private:
        const Sequence<F>& sequence;
    };
}

#endif