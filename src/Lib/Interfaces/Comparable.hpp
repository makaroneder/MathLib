#ifndef MathLib_Interfaces_Comparable_H
#define MathLib_Interfaces_Comparable_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    struct Comparable : Allocatable {
        [[nodiscard]] bool operator==(const Comparable& other) const {
            return Equals(other);
        }
        [[nodiscard]] bool operator!=(const Comparable& other) const {
            return !Equals(other);
        }

        protected:
        [[nodiscard]] virtual bool Equals(const Comparable& other) const = 0;
    };
}

#endif