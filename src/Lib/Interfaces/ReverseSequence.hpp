#ifndef MathLib_Interfaces_ReverseSequence_H
#define MathLib_Interfaces_ReverseSequence_H
#include "Sequence.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct ReverseSequence : Sequence<T> {
        ReverseSequence(const Sequence<T>& sequence) : sequence(sequence) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(sequence.GetSize());
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(sequence.At(sequence.GetSize() - index - 1));
        }

        private:
        const Sequence<T>& sequence;
    };
}

#endif