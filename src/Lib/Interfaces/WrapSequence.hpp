#ifndef MathLib_Interfaces_WrapSequence_H
#define MathLib_Interfaces_WrapSequence_H
#include "Sequence.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    template <typename T>
    struct WrapSequence : Sequence<T> {
        WrapSequence(const Sequence<T>& sequence, size_t size) : sequence(sequence), size(size) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(index < GetSize() ? sequence.At(index % sequence.GetSize()) : T());
        }

        private:
        const Sequence<T>& sequence;
        size_t size;
    };
}

#endif