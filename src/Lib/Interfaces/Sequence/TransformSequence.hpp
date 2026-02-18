#ifndef MathLib_Interfaces_Sequence_Transform_H
#define MathLib_Interfaces_Sequence_Transform_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T, typename F>
    struct TransformSequence : Sequence<T> {
        TransformSequence(const Sequence<F>& sequence, const Function<T, F>& function) : sequence(sequence), function(function) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return function(sequence.AtUnsafe(index));
        }

        private:
        const Sequence<F>& sequence;
        const Function<T, F>& function;
    };
}

#endif