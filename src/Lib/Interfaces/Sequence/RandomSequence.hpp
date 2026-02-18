#ifndef MathLib_Interfaces_Sequence_Random_H
#define MathLib_Interfaces_Sequence_Random_H
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct RandomSequence : Sequence<T> {
        RandomSequence(const Interval<T>& interval, size_t size) : interval(interval), size(size) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t) const override {
            return interval.Random();
        }

        private:
        Interval<T> interval;
        size_t size;
    };
}

#endif