#ifndef MathLib_Interfaces_Sequence_WritableTransform_H
#define MathLib_Interfaces_Sequence_WritableTransform_H
#include "WritableSequence.hpp"

namespace MathLib {
    template <typename T, typename F>
    struct WritableTransformSequence : WritableSequence<T> {
        WritableTransformSequence(const WritableSequence<F>& sequence, const Function<T, F>& function, const Function<F, T>& inverseFunction) : sequence(sequence), function(function), inverseFunction(inverseFunction) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return sequence.GetSize();
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return function(sequence.AtUnsafe(index));
        }
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
            return sequence.SetUnsafe(index, inverseFunction(value));
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            return sequence.Add(inverseFunction(value));
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return sequence.Reset();
        }

        private:
        const WritableSequence<F>& sequence;
        const Function<T, F>& function;
        const Function<F, T>& inverseFunction;
    };
}

#endif