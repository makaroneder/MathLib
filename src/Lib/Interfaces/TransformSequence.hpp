#ifndef MathLib_Interfaces_TransformSequence_H
#define MathLib_Interfaces_TransformSequence_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T, typename F>
    struct TransformSequence : Sequence<T> {
        TransformSequence(const Sequence<F>& sequence, const Function<T, F>& function) : sequence(sequence), function(function) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(sequence.GetSize());
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(index < GetSize() ? function(sequence.At(index)) : T());
        }

        private:
        const Sequence<F>& sequence;
        const Function<T, F>& function;
    };
}

#endif