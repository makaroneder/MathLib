#ifndef MathLib_Interfaces_SubSequence_H
#define MathLib_Interfaces_SubSequence_H
#include "Sequence.hpp"
#include "../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct SubSequence : Sequence<T> {
        SubSequence(const Sequence<T>& sequence, const Interval<size_t>& interval) : sequence(sequence), interval(interval) {
            EmptyBenchmark
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
            EndBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(interval.GetSize());
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(index < GetSize() ? sequence.At(interval.GetMin() + index) : T());
        }

        private:
        const Sequence<T>& sequence;
        Interval<size_t> interval;
    };
}

#endif