#ifndef MathLib_Interfaces_Sequence_Substitution_H
#define MathLib_Interfaces_Sequence_Substitution_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct SequenceSubstitution : Sequence<T> {
        SequenceSubstitution(const Sequence<T>& sequence, const T& value, size_t index) : sequence(sequence), value(value), index(index) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t i) const override {
            return i == index ? value : sequence.AtUnsafe(i);
        }

        private:
        const Sequence<T>& sequence;
        T value;
        size_t index;
    };
}

#endif