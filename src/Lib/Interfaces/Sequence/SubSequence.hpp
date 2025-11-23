#ifndef MathLib_Interfaces_Sequence_Sub_H
#define MathLib_Interfaces_Sequence_Sub_H
#include "Sequence.hpp"
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct SubSequence : Sequence<T> {
        SubSequence(const Sequence<T>& sequence, const Interval<size_t>& interval) : interval(interval), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(interval.GetMin() + index);
        }

        private:
        Interval<size_t> interval;
        const Sequence<T>& sequence;
    };
}

#endif