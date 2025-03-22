#ifndef MathLib_Interfaces_EmptySequence_H
#define MathLib_Interfaces_EmptySequence_H
#include "Sequence.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct EmptySequence : Sequence<T> {
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(0);
        }
        [[nodiscard]] virtual T At(size_t) const override {
            StartAndReturnFromBenchmark(T());
        }
    };
}

#endif