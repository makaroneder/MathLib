#ifndef MathLib_Interfaces_Sequence_WritableSub_H
#define MathLib_Interfaces_Sequence_WritableSub_H
#include "WritableSequence.hpp"
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct WritableSubSequence : WritableSequence<T> {
        WritableSubSequence(const WritableSequence<T>& sequence, const Interval<size_t>& interval) : interval(interval), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(interval.GetMin() + index);
        }
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
            return sequence.SetUnsafe(interval.GetMin() + index, value);
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
        const WritableSequence<T>& sequence;
    };
}

#endif