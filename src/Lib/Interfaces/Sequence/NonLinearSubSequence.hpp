#ifndef MathLib_Interfaces_Sequence_NonLinearSub_H
#define MathLib_Interfaces_Sequence_NonLinearSub_H
#include "NonLinearCollection.hpp"
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct NonLinearSubSequence : NonLinearCollection<T> {
        NonLinearSubSequence(const NonLinearCollection<T>& sequence, const Interval<size_t>& interval) : interval(interval), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(interval.GetMin() + index);
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            return sequence.AtUnsafe(interval.GetMin() + index);
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            if (interval.GetMax() == sequence.GetSize() && !sequence.Add(value)) return false;
            interval.max++;
            return true;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            if (!sequence.Reset()) return false;
            interval = Interval<size_t>(0, 0);
            return true;
        }

        private:
        Interval<size_t> interval;
        const NonLinearCollection<T>& sequence;
    };
}

#endif