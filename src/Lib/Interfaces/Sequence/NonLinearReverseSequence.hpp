#ifndef MathLib_Interfaces_Sequence_NonLinearReverse_H
#define MathLib_Interfaces_Sequence_NonLinearReverse_H
#include "NonLinearCollection.hpp"

namespace MathLib {
    template <typename T>
    struct NonLinearReverseSequence : NonLinearCollection<T> {
        NonLinearReverseSequence(const NonLinearCollection<T>& sequence) : sequence(sequence) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(sequence.GetSize() - index - 1);
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            return sequence.AtUnsafe(sequence.GetSize() - index - 1);
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            if (!sequence.Add(value)) return false;
            const size_t size = sequence.GetSize();
            for (size_t i = size - 1; i; i--)
                if (!sequence.SetUnsafe(i, sequence.AtUnsafe(i - 1))) return false;
            return sequence.SetUnsafe(0, value);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return sequence.Reset();
        }

        private:
        const NonLinearCollection<T>& sequence;
    };
}

#endif