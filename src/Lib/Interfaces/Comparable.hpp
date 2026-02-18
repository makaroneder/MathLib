#ifndef MathLib_Interfaces_Comparable_H
#define MathLib_Interfaces_Comparable_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename Self>
    struct Comparable : Allocatable {
        [[nodiscard]] bool operator==(const Self& other) const {
            return Equals(other);
        }
        [[nodiscard]] bool operator!=(const Self& other) const {
            return !Equals(other);
        }

        protected:
        [[nodiscard]] virtual bool Equals(const Self& other) const = 0;
    };
}

#endif