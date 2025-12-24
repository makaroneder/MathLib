#ifndef MathLib_Interfaces_Sequence_Union_H
#define MathLib_Interfaces_Sequence_Union_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct SequenceUnion : Sequence<T> {
        SequenceUnion(const Sequence<T>& sequence1, const Sequence<T>& sequence2) : sequence1(sequence1), sequence2(sequence2) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence1.GetSize() + sequence2.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            const size_t tmp = sequence1.GetSize();
            return index < tmp ? sequence1.AtUnsafe(index) : sequence2.AtUnsafe(index - tmp);
        }

        private:
        const Sequence<T>& sequence1;
        const Sequence<T>& sequence2;
    };
}

#endif