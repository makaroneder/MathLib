#ifndef MathLib_Interfaces_Container_H
#define MathLib_Interfaces_Container_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename T>
    struct Container : Allocatable {
        [[nodiscard]] virtual bool Contains(const T& value) const = 0;
    };
}

#endif