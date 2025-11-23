#ifndef MathLib_Interfaces_Sequence_SubCollection_H
#define MathLib_Interfaces_Sequence_SubCollection_H
#include "Collection.hpp"
#include "../../Interval.hpp"

namespace MathLib {
    template <typename T>
    struct SubCollection : Collection<T> {
        SubCollection(const Collection<T>& sequence, const Interval<size_t>& interval) : interval(interval), sequence(sequence) {
            if (interval.GetMax() > sequence.GetSize()) Panic("Sub sequence is out of bounds");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return interval.GetSize();
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return sequence.GetValue() + interval.GetMin();
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return sequence.GetValue() + interval.GetMin();
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
        const Collection<T>& sequence;
    };
}

#endif