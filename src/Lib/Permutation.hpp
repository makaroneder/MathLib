#ifndef MathLib_Permutation_H
#define MathLib_Permutation_H
#include "Expected.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] Expected<bool> NextPermutation(Sequence<T>& array) {
        StartBenchmark
        if (array.GetSize() < 2) ReturnFromBenchmark(Expected<bool>(false));
        size_t i = array.GetSize() - 2;
        while (array.At(i) >= array.At(i + 1)) {
            if (!i) ReturnFromBenchmark(Expected<bool>(false));
            i--;
        }
        size_t j = array.GetSize() - 1;
        while (array.At(j) <= array.At(i)) j--;
        Swap<T>(array.At(i), array.At(j));
        ReturnFromBenchmark(array.Reverse(i + 1, array.GetSize()) ? Expected<bool>(true) : Expected<bool>());
    }
}

#endif