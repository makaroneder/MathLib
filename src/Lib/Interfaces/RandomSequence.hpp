#ifndef MathLib_Interfaces_RandomSequence_H
#define MathLib_Interfaces_RandomSequence_H
#include "../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct RandomSequence : Sequence<T> {
        RandomSequence(const Interval<T>& interval, size_t size) : interval(interval), size(size) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(index < GetSize() ? interval.Random() : T());
        }

        private:
        Interval<T> interval;
        size_t size;
    };
}

#endif