#ifndef MathLib_Interfaces_FunctionSequence_H
#define MathLib_Interfaces_FunctionSequence_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct FunctionSequence : Sequence<T> {
        FunctionSequence(const Function<T, size_t>& function, size_t size) : function(function), size(size) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(function(index));
        }

        private:
        const Function<T, size_t>& function;
        size_t size;
    };
}

#endif