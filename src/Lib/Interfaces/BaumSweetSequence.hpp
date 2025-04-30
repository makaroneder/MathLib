#ifndef MathLib_Interfaces_BaumSweetSequence_H
#define MathLib_Interfaces_BaumSweetSequence_H
#include "../Typedefs.hpp"

namespace MathLib {
    struct BaumSweetSequence : Function<bool, size_t> {
        [[nodiscard]] virtual bool Invoke(const void* data, size_t index) const override {
            StartBenchmark
            if (!index) ReturnFromBenchmark(true);
            while (!(index % 4)) index /= 4;
            ReturnFromBenchmark(index % 2 && Invoke(data, (index - 1) / 2));
        }
    };
}

#endif