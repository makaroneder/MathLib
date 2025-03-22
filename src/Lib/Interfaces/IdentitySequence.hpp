#ifndef MathLib_Interfaces_IdentitySequence_H
#define MathLib_Interfaces_IdentitySequence_H
#include "Sequence.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct IdentitySequence : Sequence<T> {
        IdentitySequence(size_t size) : size(size) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(index < size ? (T)index : T());
        }
    
        private:
        size_t size;
    };
}

#endif