#ifndef MathLib_Interfaces_Sequence_Sub_H
#define MathLib_Interfaces_Sequence_Sub_H
#include "Sequence.hpp"
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct SubSequence : Sequence<T> {
        SubSequence(const Sequence<T>& sequence, size_t start, size_t end) : interval(start, end), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        SubSequence(const Sequence<T>& sequence, const Interval<size_t>& interval) : interval(interval), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(interval.GetMin() + index);
        }
        [[nodiscard]] SubSequence<T> Restrict(const Interval<size_t>& restriction) const {
            if (restriction.GetMax() > GetSize()) Panic("Restriction is out of bounds");
            return SubSequence<T>(sequence, Interval<size_t>(interval.GetMin() + restriction.GetMin(), interval.GetMin() + restriction.GetMax()));
        }
        [[nodiscard]] SubSequence<T> Restrict(size_t start, size_t end) const {
            if (end > GetSize()) Panic("Restriction is out of bounds");
            return SubSequence<T>(sequence, Interval<size_t>(interval.GetMin() + start, interval.GetMin() + end));
        }

        private:
        Interval<size_t> interval;
        const Sequence<T>& sequence;
    };
}

#endif