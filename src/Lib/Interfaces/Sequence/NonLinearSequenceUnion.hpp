#ifndef MathLib_Interfaces_Sequence_NonLinearUnion_H
#define MathLib_Interfaces_Sequence_NonLinearUnion_H
#include "NonLinearCollection.hpp"

namespace MathLib {
    template <typename T>
    struct NonLinearSequenceUnion : NonLinearCollection<T> {
        NonLinearSequenceUnion(const NonLinearCollection<T>& sequence1, const NonLinearCollection<T>& sequence2) : sequence1(sequence1), sequence2(sequence2) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence1.GetSize() + sequence2.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            const size_t tmp = sequence1.GetSize();
            return index < tmp ? sequence1.AtUnsafe(index) : sequence2.AtUnsafe(index - tmp);
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            return sequence2.Add(value);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return sequence1.Reset() && sequence2.Reset();
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            const size_t tmp = sequence1.GetSize();
            return index < tmp ? sequence1.AtUnsafe(index) : sequence2.AtUnsafe(index - tmp);
        }

        private:
        const NonLinearCollection<T>& sequence1;
        const NonLinearCollection<T>& sequence2;
    };
}

#endif