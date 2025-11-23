#ifndef MathLib_Permutation_H
#define MathLib_Permutation_H
#include "Expected.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] Expected<bool> NextPermutation(Sequence<T>& array) {
        if (array.GetSize() < 2) return false;
        size_t i = array.GetSize() - 2;
        while (array.AtUnsafe(i) >= array.AtUnsafe(i + 1)) {
            if (!i) return false;
            i--;
        }
        size_t j = array.GetSize() - 1;
        while (array.AtUnsafe(j) <= array.AtUnsafe(i)) j--;
        Swap<T>(array.AtUnsafe(i), array.AtUnsafe(j));
        return array.Reverse(i + 1, array.GetSize()) ? true : Expected<bool>();
    }
}

#endif