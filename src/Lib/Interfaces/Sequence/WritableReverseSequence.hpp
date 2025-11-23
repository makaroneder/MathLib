#ifndef MathLib_Interfaces_Sequence_WritableReverse_H
#define MathLib_Interfaces_Sequence_WritableReverse_H
#include "WritableSequence.hpp"

namespace MathLib {
    template <typename T>
    struct WritableReverseSequence : WritableSequence<T> {
        WritableReverseSequence(const WritableSequence<T>& sequence) : sequence(sequence) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return sequence.AtUnsafe(sequence.GetSize() - index - 1);
        }
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
            return sequence.SetUnsafe(sequence.GetSize() - index - 1, value);
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
        const WritableSequence<T>& sequence;
    };
}

#endif