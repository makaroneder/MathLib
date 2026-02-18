#ifndef MathLib_Interfaces_Sequence_Function_H
#define MathLib_Interfaces_Sequence_Function_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct FunctionSequence : Sequence<T> {
        FunctionSequence(const Function<T, size_t>& function, size_t size) : function(function), size(size) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return size;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return function(index);
        }

        private:
        const Function<T, size_t>& function;
        size_t size;
    };
}

#endif