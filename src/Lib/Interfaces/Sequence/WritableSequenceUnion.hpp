#ifndef MathLib_Interfaces_Sequence_WritableUnion_H
#define MathLib_Interfaces_Sequence_WritableUnion_H
#include "WritableSequence.hpp"

namespace MathLib {
    template <typename T>
    struct WritableSequenceUnion : WritableSequence<T> {
        WritableSequenceUnion(const WritableSequence<T>& sequence1, const WritableSequence<T>& sequence2) : sequence1(sequence1), sequence2(sequence2) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence1.GetSize() + sequence2.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            const size_t tmp = sequence1.GetSize();
            return index < tmp ? sequence1.AtUnsafe(index) : sequence2.AtUnsafe(index - tmp);
        }
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
            const size_t tmp = sequence1.GetSize();
            return index < tmp ? sequence1.SetUnsafe(index, value) : sequence2.SetUnsafe(index - tmp, value);
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            return sequence2.Add(value);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return sequence1.Reset() && sequence2.Reset();
        }

        private:
        const WritableSequence<T>& sequence1;
        const WritableSequence<T>& sequence2;
    };
}

#endif