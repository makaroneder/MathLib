#ifndef MathLib_Interfaces_Sequence_Interval_H
#define MathLib_Interfaces_Sequence_Interval_H
#include "../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct IntervalSequence : Sequence<T> {
        IntervalSequence(const Interval<T>& interval, const T& step) : interval(interval), step(step) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize() / step;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return interval.GetMin() + step * index;
        }
    
        private:
        Interval<T> interval;
        T step;
    };
}

#endif