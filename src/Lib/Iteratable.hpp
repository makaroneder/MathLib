#ifndef MathLib_Iteratable_H
#define MathLib_Iteratable_H
#include "Iterator.hpp"

namespace MathLib {
    template <typename T>
    struct Iteratable : Allocatable {
        [[nodiscard]] virtual Iterator<const T> begin(void) const = 0;
        [[nodiscard]] virtual Iterator<const T> end(void) const = 0;
        [[nodiscard]] virtual Iterator<T> begin(void) = 0;
        [[nodiscard]] virtual Iterator<T> end(void) = 0;
    };
}

#endif